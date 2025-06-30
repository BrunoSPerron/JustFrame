// Every Frame is a Choice //
#pragma once

#include "CoreMinimal.h"
#include "Game/GameManagerSubsystem.h"
#include "Systems/Player/PlayerSettingsManager.h"
#include "Systems/Input/InputBufferManager.h"

class IMenuInputCore {
public:
  virtual ~IMenuInputCore() = default;

  void TriggerInputs(UObject *ContextObject);

  void SetInputActive(bool bEnable) { bIsActive = bEnable; }

  virtual void ProcessInput(uint8 PlayerID, uint16 InputMask) = 0;

protected:
  bool bIsActive = false;
};
