// Every Frame is a Choice //
#pragma once

#include <SDL3/SDL.h>

class IInputConsumer {
public:
  virtual ~IInputConsumer() {}
  virtual void ConsumeInput(SDL_JoystickID JoyID, const TSet<SDL_GamepadButton> &Held,
                            const TSet<SDL_GamepadButton> &Pressed,
                            const TSet<SDL_GamepadButton> &Released, uint32 Frame) = 0;
};
