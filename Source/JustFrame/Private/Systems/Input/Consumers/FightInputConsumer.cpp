// Every Frame is a Choice //
#include "Systems/Input/Consumers/FightInputConsumer.h"
#include "Data/LogCategories.h"

void FFightInputConsumer::ConsumeInput(SDL_JoystickID JoyID, const TSet<SDL_GamepadButton> &Held,
                                       const TSet<SDL_GamepadButton> &Pressed,
                                       const TSet<SDL_GamepadButton> &Released, uint32 Frame) {
  const TMap<SDL_JoystickID, uint8> &Mapping = PlayerSettings->GetJoystickToPlayer();
  if (!Mapping.Contains(JoyID)) {
    UE_LOG(InputLog, Warning, TEXT("ConsumeInput: Unknown JoyID %d"), JoyID);
    return;
  }

  const uint8 PlayerID = Mapping[JoyID];
  const FInputMapping &InputMap = PlayerSettings->GetInputMapping(PlayerID);

  uint16 InputMask = 0;
  //UE_LOG(InputLog, Verbose, TEXT("ConsumeInput: PlayerID %d, Frame %u"), PlayerID, Frame);

  for (const TPair<SDL_GamepadButton, uint16> &Pair : InputMap.SDLButtonToBitMask) {
    const SDL_GamepadButton Button = Pair.Key;
    const uint16 Bit = Pair.Value;

    const bool bHeld = Held.Contains(Button);
    const bool bPressed = Pressed.Contains(Button);
    const bool bReleased = Released.Contains(Button);

    //UE_LOG(InputLog, VeryVerbose, TEXT("  Button %d - Held: %d, Pressed: %d, Released: %d"),
    //       static_cast<int32>(Button), bHeld, bPressed, bReleased);

    if (!(bHeld && bPressed) && ((bHeld && !bReleased) || bPressed)) {
      InputMask |= Bit;
      //UE_LOG(InputLog, VeryVerbose, TEXT("    -> Bit %d added to InputMask"), Bit);
    }
  }

  //UE_LOG(InputLog, Verbose, TEXT("Final InputMask for Player %d: 0x%04X"), PlayerID, InputMask);
  InputBufferManager->InjectInput(PlayerID, InputMask, Frame);
}
