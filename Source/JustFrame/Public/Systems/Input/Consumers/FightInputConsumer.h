// Every Frame is a Choice //
#pragma once

#include "Systems/Input/IInputConsumer.h"
#include "Systems/Input/InputBufferManager.h"
#include "Systems/Player/PlayerSettingsManager.h"

class FFightInputConsumer : public IInputConsumer {
public:
  FFightInputConsumer(UInputBufferManager *InInputBufferManager,
                      UPlayerSettingsManager *InPlayerSettings)
      : InputBufferManager(InInputBufferManager), PlayerSettings(InPlayerSettings) {}

  virtual void ConsumeInput(SDL_JoystickID JoyID, const TSet<SDL_GamepadButton> &Held,
                            const TSet<SDL_GamepadButton> &Pressed,
                            const TSet<SDL_GamepadButton> &Released, uint32 Frame) override;

private:
  UInputBufferManager *InputBufferManager;
  UPlayerSettingsManager *PlayerSettings;
};
