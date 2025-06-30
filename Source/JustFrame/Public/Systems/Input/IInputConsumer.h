// Every Frame is a Choice //
#pragma once

#include <SDL3/SDL.h>

class IInputConsumer {
public:
  virtual ~IInputConsumer() {}
  virtual void ConsumeInput(uint8 PlayerID, const TSet<SDL_GamepadButton> &Held,
                            const TSet<SDL_GamepadButton> &Pressed,
                            const TSet<SDL_GamepadButton> &Released) = 0;
};
