// Every Frame is a Choice
#pragma once

#include "IInputWorker.h"
#include <atomic>
#include <condition_variable>
#include <queue>
#include <mutex>
#include <SDL3/SDL.h>
#include <thread>
#include <unordered_set>

enum class EDaemonMessageType : uint32_t {
  SDL_Event = 0,
  JoystickListUpdate = 1,
};

struct FJoystickListUpdate {
  uint32_t Count;
  SDL_JoystickID IDs[16];
};

class FSDLInputDaemonWorker : public IInputWorker {
public:
  FSDLInputDaemonWorker(const FString &InSocketPath);
  ~FSDLInputDaemonWorker();

  void CleanUp();
  std::vector<SDL_JoystickID> GetJoysticks() const;
  void Start() override;
  void Stop() override;
  bool Dequeue(SDL_Event &OutEvent) override;
  void HandleConnected(const SDL_Event &Event);
  void HandleDisconnected(const SDL_Event &Event);

private:
  void Run();
  bool ConnectToDaemon();
  void HandleSDLEventFrame(uint32_t FrameSize);
  void HandleJoystickListUpdate(uint32_t FrameSize);

  FString SocketPath;
  std::atomic<bool> bRunning;
  std::thread WorkerThread;

  int SocketFD = -1;
  std::mutex QueueMutex;
  std::condition_variable DataAvailable;
  std::queue<SDL_Event> EventQueue;

  std::unordered_set<SDL_JoystickID> ConnectedJoysticks;
  mutable std::mutex JoystickMutex;
};
