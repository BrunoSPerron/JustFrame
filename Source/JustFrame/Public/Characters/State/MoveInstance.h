// Every Frame is a Choice //
#pragma once

#include "CoreMinimal.h"
#include "Data/Structs_Move.h"

struct FMoveInstance {
  const FMoveData *Move = nullptr;
  int32 CurrentFrame = 0;

  bool bMoveConnected = false;

  bool IsStartup() const;
  bool IsActive() const;
  bool IsRecovery() const;
  bool IsDone() const;

  void Tick();
};