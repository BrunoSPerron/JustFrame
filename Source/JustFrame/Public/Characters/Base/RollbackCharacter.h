// Every Frame is a Choice //
#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Pawn.h"
#include "RollbackCharacter.generated.h"

// Rollback state — update when adding gameplay fields.
//  Use: FVector, float, int32, enums, etc.
//  NO: TArray, FString, pointers, UObjects, or anything non-deterministic.
USTRUCT()
struct FCharacterState {
  GENERATED_BODY()

  FVector Position;
  FRotator Rotation;
  FVector Velocity;
  uint8 Health;
  uint8 NeutralStanceIndex;
};

UCLASS()
class JUSTFRAME_API ARollbackCharacter : public APawn {
  GENERATED_BODY()

public:
  ARollbackCharacter();

  virtual void BeginPlay() override;
  virtual void SimulateFrame(float DeltaTime, const uint16 InputMask);
  void SaveState(TArray<uint8> &OutData) const;
  void LoadState(const TArray<uint8> &InData);
  void SetTarget(ARollbackCharacter *Target);

  const FCharacterState *GetSimState();

  UPROPERTY(VisibleAnywhere)
  UCapsuleComponent *CapsuleComponent;

  UPROPERTY(VisibleAnywhere)
  USkeletalMeshComponent *Mesh;

protected:
  FCharacterState SimState;
  void ApplyInput(const uint16 InputMask, float DeltaTime);
  void UpdateMovement(float DeltaTime);
  ARollbackCharacter *target;
};
