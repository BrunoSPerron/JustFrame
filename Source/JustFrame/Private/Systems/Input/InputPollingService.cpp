// Every Frame is a Choice //
#include "Systems/Input/InputPollingService.h"
#include "Data/Structs/InputEnums.h"
#include "Data/LogCategories.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Systems/Input/InputBufferManager.h"
#include "Systems/Input/Workers/FSDLInputDaemonWorker.h"
#include "Systems/Player/PlayerSettingsManager.h"
#include <SDL3/SDL.h>
#include "Systems/Rollback/RollbackSimulationManager.h"
#include "Systems/Input/InputRouter.h"

void UInputPollingService::Init(UInputBufferManager *InBufferManager,
                                UPlayerSettingsManager *InPlayerSettings,
                                URollbackSimulationManager *InRollbackSimManager) {
  InputBufferManager = InBufferManager;
  PlayerSettingsManager = InPlayerSettings;
  RollbackSimManager = InRollbackSimManager;

  SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI, "0");
  SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");

  const char *SocketPath = "/tmp/sdlinput.sock";

  if (access(SocketPath, F_OK) == 0) {
    SDLWorker = MakeUnique<FSDLInputDaemonWorker>(FString(SocketPath));
    UE_LOG(InputLog, Log,
           TEXT("UInputPollingService::Init Using SDLInputDaemonWorker via socket: %s"),
           *FString(SocketPath));
  } else {
#if WITH_EDITOR
    UE_LOG(InputLog, Warning,
           TEXT("UInputPollingService::Init SDLInputDaemon not found at %s — waiting for daemon to "
                "launch..."),
           *FString(SocketPath));

    bool bSocketExists = false;
    while (!bSocketExists) {
      FPlatformProcess::Sleep(3);
      bSocketExists = access(SocketPath, F_OK) == 0;
    }

    UE_LOG(InputLog, Log,
           TEXT("UInputPollingService::Init ...SDLInputDaemon detected after wait."));
    SDLWorker = MakeUnique<FSDLInputDaemonWorker>(FString(SocketPath));

#else
    SDLWorker = MakeUnique<FSDLInputWorker>();
    UE_LOG(InputLog, Log,
           TEXT("UInputPollingService::Init Using FSDLInputWorker (no daemon detected)."));
#endif
  }

  SDLWorker->Start();

  // Enumerate already-connected controllers
  std::vector<SDL_JoystickID> joysticks = SDLWorker->GetJoysticks();
  for (SDL_JoystickID id : joysticks) {
    PlayerSettingsManager->OnControllerConnected(id);
    UE_LOG(InputLog, Log, TEXT("UInputPollingService::Init Gamepad %d initialized on startup"), id);
  }

  UE_LOG(LogTemp, Log, TEXT("UInputPollingService::Init initialized with SDLWorker ptr: %p"),
         SDLWorker.Get());
}

void UInputPollingService::PrepareNextFrame() {
  PressedThisFrame.Empty();
  ReleasedThisFrame.Empty();
  HeldAtFrameStart = CurrentInputState;
}

void UInputPollingService::PollControllers() {
  uint32 CurrentFrame = RollbackSimManager->GetCurrentFrame();
  /*UE_LOG(InputLog, Log, TEXT("UInputPollingService::PollControllers - Frame %u start"), CurrentFrame);

  if (!SDLWorker) {
    UE_LOG(LogTemp, Error, TEXT("UInputPollingService::PollControllers SDLWorker is null!"));
  } else {
    UE_LOG(LogTemp, Log, TEXT("UInputPollingService::PollControllers SDLWorker is valid at frame %u"), CurrentFrame);
  }*/

  SDL_Event Event;
  /*bool bGotEvent = SDLWorker->Dequeue(Event);
  UE_LOG(LogTemp, Log, TEXT("UInputPollingService::PollControllers Initial Dequeue call returned: %s"),
         bGotEvent ? TEXT("true") : TEXT("false"));
  if (bGotEvent) HandleSDLEvent(Event);*/
  while (SDLWorker->Dequeue(Event)) {
    HandleSDLEvent(Event);
  }

  const TMap<SDL_JoystickID, uint8> JoystickToPlayer = PlayerSettingsManager->GetJoystickToPlayer();
  for (const TPair<SDL_JoystickID, uint8> &Pair : JoystickToPlayer) {
    SDL_JoystickID JoyID = Pair.Key;
    uint8 PlayerID = Pair.Value;

    const FInputMapping &Mapping = PlayerSettingsManager->GetInputMapping(PlayerID);

    const TSet<SDL_GamepadButton> &Held = HeldAtFrameStart.FindRef(JoyID);
    const TSet<SDL_GamepadButton> &Pressed = PressedThisFrame.FindRef(JoyID);
    const TSet<SDL_GamepadButton> &Released = ReleasedThisFrame.FindRef(JoyID);

    uint16 InputMask = 0;

    //FString PressedButton;

    for (const TPair<SDL_GamepadButton, uint16> &MapPair : Mapping.SDLButtonToBitMask) {
      SDL_GamepadButton Button = MapPair.Key;
      uint16 Bit = MapPair.Value;

      const bool bHeld = Held.Contains(Button);
      const bool bPressed = Pressed.Contains(Button);
      const bool bReleased = Released.Contains(Button);

      if (!(bHeld && bPressed) && ((bHeld && !bReleased) || bPressed)) {
        InputMask |= Bit;

        /*// Map input bit to name string for logging (Keep)
        // clang-format off
        FString BitName;
        switch (static_cast<EInputBit>(Bit)) {
          case EInputBit::Up: BitName = TEXT("Up"); break;
          case EInputBit::Down: BitName = TEXT("Down"); break;
          case EInputBit::Left: BitName = TEXT("Left"); break;
          case EInputBit::Right: BitName = TEXT("Right"); break;
          case EInputBit::Low: BitName = TEXT("Low"); break;
          case EInputBit::High: BitName = TEXT("High"); break;
          case EInputBit::Punch: BitName = TEXT("Punch"); break;
          case EInputBit::Kick: BitName = TEXT("Kick"); break;
          case EInputBit::Guard: BitName = TEXT("Guard"); break;
          case EInputBit::Step: BitName = TEXT("Step"); break;
          default: BitName = FString::Printf(TEXT("UnknownBit(%d)"), Bit); break;
        }
        // clang-format on 
        PressedButton += BitName + ",";*/
      }
    }
    if (InputRouter) {
      InputRouter->Route(JoyID, Held, Pressed, Released, CurrentFrame);
    }
    //UE_LOG(InputLog, Log, TEXT("PollControllers - Inputs: %s"), *PressedButton);
  }
  PrepareNextFrame();
  UE_LOG(InputLog, Log, TEXT("UInputPollingService::PollControllers - end"));
}

void UInputPollingService::HandleSDLEvent(const SDL_Event &Event) {
  //UE_LOG(InputLog, Log, TEXT("UInputPollingService::HandleSDLEvent"));
  switch (Event.type) {
    case SDL_EVENT_GAMEPAD_ADDED:
      SDLWorker->HandleConnected(Event);
      PlayerSettingsManager->OnControllerConnected(Event.gdevice.which);
      UE_LOG(InputLog, Log, TEXT("UInputPollingService::HandleSDLEvent Gamepad %d connected"),
             Event.gdevice.which);
      break;

    case SDL_EVENT_GAMEPAD_REMOVED:
      SDLWorker->HandleDisconnected(Event);
      PlayerSettingsManager->OnControllerDisconnected(Event.gdevice.which);
      UE_LOG(InputLog, Log, TEXT("UInputPollingService::HandleSDLEvent Gamepad %d disconnected"),
             Event.gdevice.which);
      break;

    case SDL_EVENT_GAMEPAD_BUTTON_DOWN: {
      SDL_JoystickID JoyID = Event.gbutton.which;
      SDL_GamepadButton Button = static_cast<SDL_GamepadButton>(Event.gbutton.button);
      PressedThisFrame.FindOrAdd(JoyID).Add(Button);
      CurrentInputState.FindOrAdd(JoyID).Add(Button);
      break;
    }

    case SDL_EVENT_GAMEPAD_BUTTON_UP: {
      SDL_JoystickID JoyID = Event.gbutton.which;
      SDL_GamepadButton Button = static_cast<SDL_GamepadButton>(Event.gbutton.button);
      ReleasedThisFrame.FindOrAdd(JoyID).Add(Button);
      CurrentInputState.FindOrAdd(JoyID).Remove(Button);
      break;
    }

    default:
      break;
  }
}

void UInputPollingService::Shutdown() {
  if (SDLWorker.IsValid()) {
    SDLWorker->Stop();
    SDLWorker->CleanUp();
    SDLWorker.Reset();
  }
}
