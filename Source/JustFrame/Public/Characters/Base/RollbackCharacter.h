// Every Frame is a Choice
#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Pawn.h"
#include "RollbackCharacter.generated.h"

// Only add Deterministic field. FVector, int32, float, enums, etc.
// No TArray, FString, pointers, UObjects, etc.
USTRUCT()
struct FCharacterState {
  GENERATED_BODY()

  FVector Position;
  FVector Velocity;
  uint8_t Health;
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

  UPROPERTY(VisibleAnywhere)
  UCapsuleComponent *CapsuleComponent;

  UPROPERTY(VisibleAnywhere)
  USkeletalMeshComponent *Mesh;

protected:
  FCharacterState SimState;
  void ApplyInput(const uint16 InputMask, float DeltaTime);
  void UpdateMovement(float DeltaTime);
};
