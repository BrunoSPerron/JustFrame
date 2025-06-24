// Every Frame is a Choice //
#pragma once

#include "CoreMinimal.h"
#include "Data/Structs/Structs_Move.h"

/* all stances must self register
#include "Characters/State/Stance/StanceRegistration.h"
REGISTER_STANCE_INSTANCE(FJumpForwardStance, TEXT("JumpForward"));
 */

class ARollbackCharacter;

class FStanceInstance {
public:
  FStanceInstance(ARollbackCharacter *InOwner, const FStanceData &InData)
      : Owner(InOwner), StanceData(InData) {}

  virtual ~FStanceInstance() = default;

  virtual void ApplyMovementInput(uint16 InputMask, float DeltaTime) {}

  virtual void OnExit() {}

  virtual bool CanBlock() const { return StanceData.CanBlock; }
  virtual bool IsAirborne() const { return StanceData.IsAirborne; }
  virtual bool IsCrouching() const { return StanceData.IsCrouching; }
  virtual int32 GetBalance() const { return StanceData.Balance; }

  virtual const TArray<FString> &GetEnterFX() const { return StanceData.TransitionFX; }
  virtual const TArray<FString> &GetAvailableMoves() const { return StanceData.AvailableMoves; }
  virtual const FString &GetStanceId() const { return StanceData.StanceId; }

  int32 GetElapsedFrames() const { return ElapsedFrames; }

protected:
  ARollbackCharacter *Owner = nullptr;
  const FStanceData StanceData;
  int32 ElapsedFrames = 0;
};
