// Every Frame is a Choice //
#pragma once

#include "CoreMinimal.h"
#include "IInputConsumer.h"
#include "Data/Structs/InputEnums.h"

class UPlayerSettingsManager;

enum class EInputMode {
  Fight,
  Menu,
};

class FInputRouter {
public:
  FInputRouter(UPlayerSettingsManager *InPlayerSettingsManager);

  void SetInputMode(uint8 PlayerID, EInputMode NewMode);
  void SetGlobalInputMode(EInputMode NewMode);

  void RegisterConsumer(EInputMode Mode, TSharedPtr<IInputConsumer> Consumer);

  void Route(SDL_JoystickID JoyID, const TSet<SDL_GamepadButton> &Held,
             const TSet<SDL_GamepadButton> &Pressed, const TSet<SDL_GamepadButton> &Released,
             uint32 Frame);

private:
  TMap<EInputMode, TSharedPtr<IInputConsumer>> Consumers;
  TMap<uint8, EInputMode> PlayerInputModes;
  UPlayerSettingsManager *PlayerSettingsManager = nullptr;
};
