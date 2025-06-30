// Every Frame is a Choice //
#pragma once

#include "Systems/Input/IInputConsumer.h"
#include "Systems/Input/InputBufferManager.h"
#include "Systems/Player/PlayerSettingsManager.h"
#include "Systems/Rollback/RollbackSimulationManager.h"

class FFightInputConsumer : public IInputConsumer {
public:
  FFightInputConsumer(UInputBufferManager *InInputBufferManager,
                      UPlayerSettingsManager *InPlayerSettings,
                      URollbackSimulationManager *InRollbackSimManager)
      : InputBufferManager(InInputBufferManager), PlayerSettings(InPlayerSettings),
        RollbackSimManager(InRollbackSimManager) {}

  void SetRollbackSimManager(URollbackSimulationManager *InRollbackSimManager) {
    RollbackSimManager = InRollbackSimManager;
  }

  virtual void ConsumeInput(uint8 PlayerID, const TSet<SDL_GamepadButton> &Held,
                            const TSet<SDL_GamepadButton> &Pressed,
                            const TSet<SDL_GamepadButton> &Released) override;

private:
  UInputBufferManager *InputBufferManager = nullptr;
  UPlayerSettingsManager *PlayerSettings = nullptr;
  URollbackSimulationManager *RollbackSimManager = nullptr;
};
