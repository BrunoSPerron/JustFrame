// Every Frame is a Choice //
#include "Systems/Input/Workers/FSDLInputWorker.h"
#include "HAL/PlatformProcess.h"

FSDLInputWorker::FSDLInputWorker() : bIsRunning(false) {}

FSDLInputWorker::~FSDLInputWorker() { Stop(); }

void FSDLInputWorker::CleanUp() {
  int count = 0;
  const SDL_JoystickID *ids = SDL_GetJoysticks(&count);
  for (int i = 0; i < count; ++i) {
    SDL_JoystickID id = ids[i];
    if (SDL_IsGamepad(id)) {
      SDL_Gamepad *pad = SDL_GetGamepadFromID(id);
      if (pad) SDL_CloseGamepad(pad);
    }
  }
}

std::vector<SDL_JoystickID> FSDLInputWorker::GetJoysticks() const {
  int count = 0;
  const SDL_JoystickID *joysticks = SDL_GetJoysticks(&count);

  std::vector<SDL_JoystickID> ids;
  if (!joysticks || count <= 0) return ids;

  ids.reserve(count);
  for (int i = 0; i < count; ++i) {
    ids.push_back(joysticks[i]);
  }

  return ids;
}

void FSDLInputWorker::Start() {

#if WITH_EDITOR
  // Adding SDL3 on linux mess with the unreal editor
  if (GIsEditor && GIsPlayInEditorWorld) return;
#endif

  if (bIsRunning) return;
  bIsRunning = true;

  SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI, "0"); // disable HIDAPI backend (still needed!)
  SDL_SetHint(SDL_HINT_JOYSTICK_LINUX_CLASSIC, "1");

  SDL_InitSubSystem(SDL_INIT_GAMEPAD | SDL_INIT_JOYSTICK);
  SDL_SetGamepadEventsEnabled(true);

  WorkerThread = std::thread(&FSDLInputWorker::ThreadLoop, this);
}

void FSDLInputWorker::Stop() {
  bIsRunning = false;
  if (WorkerThread.joinable()) {
    WorkerThread.join();
  }
  SDL_QuitSubSystem(SDL_INIT_GAMEPAD | SDL_INIT_JOYSTICK);
}

bool FSDLInputWorker::Dequeue(SDL_Event &OutEvent) { return EventQueue.Dequeue(OutEvent); }

void FSDLInputWorker::ThreadLoop() {
  SDL_Event Event;
  while (SDL_PollEvent(&Event)) {
    SDL_JoystickID id = -1;

    switch (Event.type) {
      case SDL_EVENT_GAMEPAD_ADDED:
      case SDL_EVENT_GAMEPAD_REMOVED:
        id = Event.gdevice.which;
        break;
      case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
      case SDL_EVENT_GAMEPAD_BUTTON_UP:
        id = Event.gbutton.which;
        break;
      default:
        continue;
    }

    if (!SDL_IsGamepad(id)) continue;

    EventQueue.Enqueue(Event);
    FPlatformProcess::Sleep(0.001f); // 1ms
  }
}

void FSDLInputWorker::HandleConnected(const SDL_Event &Event) {
  const SDL_JoystickID ID = Event.gdevice.which;
  if (!SDL_IsGamepad(ID)) return;

  SDL_Gamepad *Controller = SDL_OpenGamepad(ID);
  if (Controller) {
    std::lock_guard<std::mutex> Lock(GamepadMutex);
    ConnectedGamepads.insert(ID);
  }
}

void FSDLInputWorker::HandleDisconnected(const SDL_Event &Event) {
  SDL_JoystickID ID = Event.gdevice.which;
  std::lock_guard<std::mutex> Lock(GamepadMutex);

  if (ConnectedGamepads.contains(ID)) {
    SDL_Gamepad *Pad = SDL_GetGamepadFromID(ID);
    if (Pad) SDL_CloseGamepad(Pad);
    ConnectedGamepads.erase(ID);
  }
}
