// Every Frame is a Choice //
#include "Systems/Input/FSDLInputDaemonWorker.h"
#include "Data/LogCategories.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

FSDLInputDaemonWorker::FSDLInputDaemonWorker(const FString &InSocketPath)
    : SocketPath(InSocketPath), bRunning(false), SocketFD(-1) {}

FSDLInputDaemonWorker::~FSDLInputDaemonWorker() { Stop(); }

void FSDLInputDaemonWorker::CleanUp() {}

void FSDLInputDaemonWorker::Start() {
  if (bRunning) return;
  bRunning = true;
  WorkerThread = std::thread(&FSDLInputDaemonWorker::Run, this);
  UE_LOG(InputLog, Log, TEXT("FSDLInputDaemonWorker::Start() called, this=%p"), this);
}

void FSDLInputDaemonWorker::Stop() {
  bRunning = false;
  if (WorkerThread.joinable()) WorkerThread.join();
  if (SocketFD != -1) {
    close(SocketFD);
    SocketFD = -1;
  }
}

bool FSDLInputDaemonWorker::Dequeue(SDL_Event &OutEvent) {
  std::unique_lock<std::mutex> Lock(QueueMutex);
  if (EventQueue.empty()) {
    //UE_LOG(InputLog, Log, TEXT("FSDLInputDaemonWorker::Dequeue() Queue is empty"));
    return false;
  }
  OutEvent = EventQueue.front();
  EventQueue.pop();
  //UE_LOG(InputLog, Log, TEXT("FSDLInputDaemonWorker::Dequeue() Event type %u dequeued"), OutEvent.type);
  return true;
}

bool FSDLInputDaemonWorker::ConnectToDaemon() {
  SocketFD = socket(AF_UNIX, SOCK_STREAM, 0);
  if (SocketFD < 0) return false;

  sockaddr_un Addr{};
  Addr.sun_family = AF_UNIX;
  strncpy(Addr.sun_path, TCHAR_TO_UTF8(*SocketPath), sizeof(Addr.sun_path) - 1);

  return connect(SocketFD, (sockaddr *)&Addr, sizeof(Addr)) == 0;
}

std::vector<SDL_JoystickID> FSDLInputDaemonWorker::GetJoysticks() const {
  std::lock_guard<std::mutex> Lock(JoystickMutex);
  return std::vector<SDL_JoystickID>(ConnectedJoysticks.begin(), ConnectedJoysticks.end());
}

// Helper: fully reads `size` bytes from socket into buffer.
static bool ReadFull(int fd, void *buffer, size_t size) {
  uint8_t *buf = static_cast<uint8_t *>(buffer);
  size_t totalRead = 0;

  while (totalRead < size) {
    ssize_t result = recv(fd, buf + totalRead, size - totalRead, 0);
    if (result <= 0) return false; // socket closed or error
    totalRead += result;
  }

  return true;
}

void FSDLInputDaemonWorker::Run() {
  UE_LOG(InputLog, Log, TEXT("FSDLInputDaemonWorker::Run() started"));

  if (!ConnectToDaemon()) {
    UE_LOG(InputLog, Error,
           TEXT("FSDLInputDaemonWorker::Run() Failed to connect to SDLInputDaemon at %s"),
           *SocketPath);
    return;
  }

  while (bRunning) {
    uint32_t FrameType = 0;
    uint32_t FrameSize = 0;

    if (!ReadFull(SocketFD, &FrameType, sizeof(FrameType)) ||
        !ReadFull(SocketFD, &FrameSize, sizeof(FrameSize))) {
      UE_LOG(InputLog, Warning,
             TEXT("FSDLInputDaemonWorker::Run() Failed to read frame header. Disconnecting."));
      break;
    }

    switch (static_cast<EDaemonMessageType>(FrameType)) {
      case EDaemonMessageType::SDL_Event:
        HandleSDLEventFrame(FrameSize);
        break;

      case EDaemonMessageType::JoystickListUpdate:
        HandleJoystickListUpdate(FrameSize);
        break;

      default:
        UE_LOG(InputLog, Warning,
               TEXT("FSDLInputDaemonWorker::Run() Unknown daemon frame type: %u"), FrameType);
        break;
    }
  }

  close(SocketFD);
  SocketFD = -1;
}

void FSDLInputDaemonWorker::HandleConnected(const SDL_Event &Event) {}
void FSDLInputDaemonWorker::HandleDisconnected(const SDL_Event &Event) {}

void FSDLInputDaemonWorker::HandleSDLEventFrame(uint32_t FrameSize) {
  if (FrameSize != sizeof(SDL_Event)) {
    UE_LOG(InputLog, Warning,
           TEXT("FSDLInputDaemonWorker::HandleSDLEventFrame() Invalid SDL_Event frame size: %u"),
           FrameSize);
    return;
  }

  SDL_Event Event;
  if (!ReadFull(SocketFD, &Event, sizeof(Event))) {
    UE_LOG(InputLog, Warning,
           TEXT("FSDLInputDaemonWorker::HandleSDLEventFrame() Failed to read SDL_Event."));
    return;
  }

  SDL_JoystickID SourceID = -1;
  switch (Event.type) {
    case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
    case SDL_EVENT_GAMEPAD_BUTTON_UP:
      SourceID = Event.gbutton.which;
      break;
    case SDL_EVENT_GAMEPAD_AXIS_MOTION:
      SourceID = Event.gaxis.which;
      break;
    case SDL_EVENT_GAMEPAD_ADDED:
    case SDL_EVENT_GAMEPAD_REMOVED:
      SourceID = Event.gdevice.which;
      break;
    default:
      return;
  }

  {
    std::lock_guard<std::mutex> Lock(JoystickMutex);

    if (Event.type == SDL_EVENT_GAMEPAD_ADDED)
      ConnectedJoysticks.insert(SourceID);
    else if (Event.type == SDL_EVENT_GAMEPAD_REMOVED)
      ConnectedJoysticks.erase(SourceID);

    if (ConnectedJoysticks.find(SourceID) == ConnectedJoysticks.end()) return;
  }

  {
    std::lock_guard<std::mutex> Lock(QueueMutex);
    EventQueue.push(Event);
  }

  /*UE_LOG(InputLog, Log, TEXT("FSDLInputDaemonWorker::HandleSDLEventFrame() Enqueued SDL_Event type: %u (SourceID: %d)"), Event.type,
         SourceID);*/

  DataAvailable.notify_one();
}

void FSDLInputDaemonWorker::HandleJoystickListUpdate(uint32_t FrameSize) {
  if (FrameSize > sizeof(FJoystickListUpdate)) {
    UE_LOG(
        InputLog, Warning,
        TEXT("FSDLInputDaemonWorker::HandleSDLEventFrame() JoystickListUpdate frame too large: %u"),
        FrameSize);
    return;
  }

  FJoystickListUpdate Payload;
  if (!ReadFull(SocketFD, &Payload, FrameSize)) {
    UE_LOG(InputLog, Warning,
           TEXT("FSDLInputDaemonWorker::HandleSDLEventFrame() Failed to read JoystickListUpdate "
                "payload."));
    return;
  }

  std::lock_guard<std::mutex> Lock(JoystickMutex);
  ConnectedJoysticks.clear();
  for (uint32_t i = 0; i < Payload.Count; ++i) {
    ConnectedJoysticks.insert(Payload.IDs[i]);
  }
}
