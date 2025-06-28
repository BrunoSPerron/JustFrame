// Every Frame is a Choice //
#pragma once

#include "Systems/Input/IInputConsumer.h"
#include <SDL3/SDL_gamepad.h>

class FMenuInputConsumer : public IInputConsumer {
public:
  virtual void ConsumeInput(SDL_JoystickID JoyID, const TSet<SDL_GamepadButton> &Held,
                            const TSet<SDL_GamepadButton> &Pressed,
                            const TSet<SDL_GamepadButton> &Released, uint32 Frame) override;
};
