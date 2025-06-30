// Every Frame is a Choice //
#pragma once

#include "CoreMinimal.h"
#include "Systems/Input/IInputConsumer.h"

class UInputBufferManager;
class UPlayerSettingsManager;

class FMenuInputConsumer : public IInputConsumer {
public:
  FMenuInputConsumer(UInputBufferManager *InBufferManager, UPlayerSettingsManager *InSettings)
      : InputBufferManager(InBufferManager), PlayerSettings(InSettings) {}

  virtual void ConsumeInput(uint8 PlayerID, const TSet<SDL_GamepadButton> &Held,
                            const TSet<SDL_GamepadButton> &Pressed,
                            const TSet<SDL_GamepadButton> &Released) override;

private:
  UInputBufferManager *InputBufferManager = nullptr;
  UPlayerSettingsManager *PlayerSettings = nullptr;
};
