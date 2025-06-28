// Every Frame is a Choice //
#include "Systems/Input/InputRouter.h"
#include "Data/LogCategories.h"
#include "Engine/Engine.h"
#include "Systems/Player/PlayerSettingsManager.h"

FInputRouter::FInputRouter(UPlayerSettingsManager *InPlayerSettingsManager)
    : PlayerSettingsManager(InPlayerSettingsManager) {
  int32 NumPlayers = PlayerSettingsManager->GetNumPlayers();
  for (int32 PlayerID = 0; PlayerID < NumPlayers; ++PlayerID) {
    SetInputMode(PlayerID, EInputMode::Menu);
  }
}

void FInputRouter::SetInputMode(uint8 PlayerID, EInputMode NewMode) {
  PlayerInputModes.Add(PlayerID, NewMode);
}

void FInputRouter::SetGlobalInputMode(EInputMode NewMode) {
  for (TPair<uint8, EInputMode> &Pair : PlayerInputModes) Pair.Value = NewMode;
}

void FInputRouter::RegisterConsumer(EInputMode Mode, TSharedPtr<IInputConsumer> Consumer) {
  Consumers.Add(Mode, Consumer);
}

void FInputRouter::Route(SDL_JoystickID JoyID, const TSet<SDL_GamepadButton> &Held,
                         const TSet<SDL_GamepadButton> &Pressed,
                         const TSet<SDL_GamepadButton> &Released, uint32 Frame) {
  if (!PlayerSettingsManager) {
    UE_LOG(InputLog, Warning, TEXT("FInputRouter::Route PlayerSettingsManager is null"));
    return;
  }

  const TMap<SDL_JoystickID, uint8> &Mapping = PlayerSettingsManager->GetJoystickToPlayer();
  const uint8 *PlayerIDPtr = Mapping.Find(JoyID);
  if (!PlayerIDPtr) {
    UE_LOG(InputLog, Warning, TEXT("FInputRouter::Route Unknown JoyID %d"), JoyID);
    return;
  }

  const uint8 PlayerID = *PlayerIDPtr;
  const EInputMode *ModePtr = PlayerInputModes.Find(PlayerID);
  const EInputMode Mode = ModePtr ? *ModePtr : EInputMode::Menu;

  /*UE_LOG(InputLog, Log, TEXT("Route: Player %d | Mode: %d | Frame: %u"), PlayerID, (int32)Mode,
         Frame);*/

  if (const TSharedPtr<IInputConsumer> *Consumer = Consumers.Find(Mode)) {
    if (Consumer->IsValid()) {
      //UE_LOG(InputLog, Verbose, TEXT("Route: Forwarding to consumer for mode %d"), (int32)Mode);
      (*Consumer)->ConsumeInput(JoyID, Held, Pressed, Released, Frame);
    } else {
      UE_LOG(InputLog, Warning, TEXT("FInputRouter::Route Consumer for mode %d is invalid"),
             (int32)Mode);
    }
  } else {
    UE_LOG(InputLog, Warning, TEXT("FInputRouter::Route No consumer found for mode %d"),
           (int32)Mode);
  }
}
