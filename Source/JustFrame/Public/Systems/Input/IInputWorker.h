// Every Frame is a Choice //
#pragma once

#include <SDL3/SDL.h>

class IInputWorker {
public:
  virtual void CleanUp() = 0;
  virtual std::vector<SDL_JoystickID> GetJoysticks() const = 0;
  virtual void Start() = 0;
  virtual void Stop() = 0;
  virtual bool Dequeue(SDL_Event &OutEvent) = 0;
  virtual void HandleConnected(const SDL_Event &Event) = 0;
  virtual void HandleDisconnected(const SDL_Event &Event) = 0;
  virtual ~IInputWorker() = default;
};