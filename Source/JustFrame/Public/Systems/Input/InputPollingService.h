// Every Frame is a Choice //
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include <SDL3/SDL.h>
#include "HAL/ThreadSafeBool.h"
#include "Containers/Queue.h"
#include "Systems/Input/Workers/FSDLInputWorker.h"
#include "Systems/Input/IInputWorker.h"
#include "InputPollingService.generated.h"

class UInputBufferManager;
class UPlayerSettingsManager;
class URollbackSimulationManager;
class FInputRouter;

struct FSDLGamepadHandle {
  SDL_Gamepad *Controller = nullptr;

  explicit FSDLGamepadHandle(SDL_Gamepad *InController) : Controller(InController) {}

  ~FSDLGamepadHandle() {
    if (Controller) {
      SDL_CloseGamepad(Controller);
      Controller = nullptr;
    }
  }
};

UCLASS()
class JUSTFRAME_API UInputPollingService : public UObject {
  GENERATED_BODY()

public:
  void Init(UInputBufferManager *InBufferManager, UPlayerSettingsManager *InPlayerSettings,
            URollbackSimulationManager *InRollbackSimManager);
  void PollControllers();
  void HandleSDLEvent(const SDL_Event &Event);
  void SetInputRouter(FInputRouter *InRouter) { InputRouter = InRouter; };
  void Shutdown();

private:
  UPROPERTY() UInputBufferManager *InputBufferManager;
  UPROPERTY() UPlayerSettingsManager *PlayerSettingsManager;
  UPROPERTY() URollbackSimulationManager *RollbackSimManager;

  void PrepareNextFrame();

  TMap<Uint32, TSet<SDL_GamepadButton>> CurrentInputState;
  TMap<Uint32, TSet<SDL_GamepadButton>> PressedThisFrame;
  TMap<Uint32, TSet<SDL_GamepadButton>> ReleasedThisFrame;
  TMap<Uint32, TSet<SDL_GamepadButton>> HeldAtFrameStart;

  TUniquePtr<IInputWorker> SDLWorker;
  FInputRouter *InputRouter = nullptr;
};