// Every Frame is a Choice
#pragma once

#include <atomic>
#include <thread>
#include "Containers/Queue.h"
#include "Systems/Input/IInputWorker.h"
#include <SDL3/SDL.h>
#include <unordered_set>

struct FSDLInputWorker : public IInputWorker {
  FSDLInputWorker();
  ~FSDLInputWorker();

  void CleanUp();
  std::vector<SDL_JoystickID> GetJoysticks() const;
  void Start();
  void Stop();
  bool Dequeue(SDL_Event &OutEvent);
  void HandleConnected(const SDL_Event &Event);
  void HandleDisconnected(const SDL_Event &Event);

private:
  void ThreadLoop();

  std::atomic<bool> bIsRunning;
  std::thread WorkerThread;
  TQueue<SDL_Event, EQueueMode::Spsc> EventQueue;

  std::unordered_set<SDL_JoystickID> ConnectedGamepads;
  std::mutex GamepadMutex;
};
