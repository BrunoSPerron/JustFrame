// Every Frame is a Choice //
#pragma once

#include "Characters/State/Stance/IStanceInstance.h"

class FNeutralStance8Way : public FStanceInstance {
public:
  FNeutralStance8Way(ARollbackCharacter *Owner, const FStanceData &Data);

  virtual void ApplyMovementInput(uint16 InputMask, float DeltaTime) override;

protected:
  FVector2D GetInputDirection() const;
};
