// Every Frame is a Choice //
#pragma once

#include "CoreMinimal.h"
#include "Characters/RollbackCharacter.h"
#include "Data/Structs/CharacterStructs.h"

/*  Stances self register using
#include "Characters/Stance/StanceRegistration.h"
REGISTER_STANCE_INSTANCE(FMyStance, TEXT("JumpForward"));
*/

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

  const FCharacterState GetTargetSimData() const;
  int32 GetElapsedFrames() const { return ElapsedFrames; }

protected:
  ARollbackCharacter *Owner = nullptr;
  const FStanceData StanceData;
  int32 ElapsedFrames = 0;
};
