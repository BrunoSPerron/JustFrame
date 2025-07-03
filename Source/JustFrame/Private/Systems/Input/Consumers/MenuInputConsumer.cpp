// Every Frame is a Choice //
#include "Systems/Input/Consumers/MenuInputConsumer.h"
#include "Systems/Input/InputBufferManager.h"
#include "Systems/Player/PlayerSettingsManager.h"
#include "Data/LogCategories.h"

void FMenuInputConsumer::ConsumeInput(uint8 PlayerID, const TSet<SDL_GamepadButton> &Held,
                                      const TSet<SDL_GamepadButton> &Pressed,
                                      const TSet<SDL_GamepadButton> &Released) {
  const FInputMapping &InputMap = PlayerSettings->GetMenuInputMapping(PlayerID);

  uint16 InputMask = 0;
  for (const TPair<SDL_GamepadButton, uint16> &Pair : InputMap.SDLButtonToBitMask) {
    const SDL_GamepadButton Button = Pair.Key;
    const uint16 Bit = Pair.Value;
    if (Pressed.Contains(Button)) {
      InputMask |= Bit;
    }
  }

  InputBufferManager->SetUnbufferedInput(PlayerID, InputMask);
}
