// Every Frame is a Choice //
#include "Systems/Input/Consumers/FightInputConsumer.h"
#include "Data/LogCategories.h"

void FFightInputConsumer::ConsumeInput(uint8 PlayerID, const TSet<SDL_GamepadButton> &Held,
                                       const TSet<SDL_GamepadButton> &Pressed,
                                       const TSet<SDL_GamepadButton> &Released) {
  const FInputMapping &InputMap = PlayerSettings->GetInputMapping(PlayerID);

  uint16 InputMask = 0;

  for (const TPair<SDL_GamepadButton, uint16> &Pair : InputMap.SDLButtonToBitMask) {
    const SDL_GamepadButton Button = Pair.Key;
    const uint16 Bit = Pair.Value;

    const bool bHeld = Held.Contains(Button);
    const bool bPressed = Pressed.Contains(Button);
    const bool bReleased = Released.Contains(Button);

    if (!(bHeld && bPressed) && ((bHeld && !bReleased) || bPressed)) {
      InputMask |= Bit;
    }
  }

  if (!RollbackSimManager) {
    UE_LOG(
        InputLog, Error,
        TEXT("FFightInputConsumer::ConsumeInput: RollbackSimManager is null! Input not injected."));
    return;
  }
  uint32 Frame = RollbackSimManager->GetCurrentFrame();
  InputBufferManager->InjectInput(PlayerID, InputMask, Frame);
}
