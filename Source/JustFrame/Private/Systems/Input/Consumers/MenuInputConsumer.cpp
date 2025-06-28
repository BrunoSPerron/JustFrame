// Every Frame is a Choice //
#include "Systems/Input/Consumers/MenuInputConsumer.h"
#include "Data/LogCategories.h"

void FMenuInputConsumer::ConsumeInput(SDL_JoystickID JoyID, const TSet<SDL_GamepadButton> &Held,
                                      const TSet<SDL_GamepadButton> &Pressed,
                                      const TSet<SDL_GamepadButton> &Released, uint32 Frame) {
  for (SDL_GamepadButton Button : Pressed) {
    FString Name = UTF8_TO_TCHAR(SDL_GetGamepadStringForButton(Button));
    UE_LOG(InputLog, Log, TEXT("FMenuInputConsumer::ConsumeInput JoyID: %d - Pressed: %s"), JoyID,
           *Name);
  }
}
