// Every Frame is a Choice //
#pragma once

#include "CoreMinimal.h"
#include "Data/Structs/CharacterEnums.h"
#include "CharacterStructs.generated.h"

USTRUCT()
struct FInputCondition {
  GENERATED_BODY()

  UPROPERTY()
  TArray<uint16> Input;
  UPROPERTY()
  bool bAllowBuffer = true;
};

USTRUCT()
struct FComboLink {
  GENERATED_BODY()

  UPROPERTY() FInputCondition InputCondition;
  UPROPERTY() FName MoveID;
  UPROPERTY() int32 WindowStart = 0;
  UPROPERTY() int32 WindowEnd = 0;
  UPROPERTY() int8 BufferInput = 0;
  UPROPERTY() int8 ChainFrameOffset = 0;
  UPROPERTY()
  bool bRequireHitConfirm = false;
};

USTRUCT()
struct FMoveData {
  GENERATED_BODY()

  UPROPERTY() FName MoveID;
  UPROPERTY() FName animation;
  UPROPERTY() uint8 StartupFrames = 0;
  UPROPERTY() int32 ActiveFrames = 0;
  UPROPERTY() int8 HitRecoveryFrames = 0;
  UPROPERTY() int8 WhiffRecoveryFrames = 0;
  UPROPERTY() int8 OnBlockAdvantage = 0;
  UPROPERTY() int8 OnHitAdvantage = 0;
  UPROPERTY() TArray<FComboLink> ComboLinks;
  UPROPERTY() EStanceState ResultingStance = EStanceState::Standing;

  UPROPERTY() TArray<uint8> BalancePerFrame;
  UPROPERTY() int8 CounterWindowModifier = 0;
  UPROPERTY() uint8 KnockbackPower = 0;
  UPROPERTY() uint8 KnockdownPower = 0;
  UPROPERTY() uint8 LaunchPower = 0;
  UPROPERTY() uint8 MoveTags; // Bitmask of EMoveTagBit values
  UPROPERTY() TArray<uint16> TrackingPerFrame;
};

USTRUCT()
struct FStanceData {
  GENERATED_BODY()

  UPROPERTY() FString Version;
  UPROPERTY() FString StanceId;
  UPROPERTY() FString AnimationSetId;
  UPROPERTY() FString StanceType;
  UPROPERTY() TArray<FString> TransitionFX;
  UPROPERTY() TArray<FString> AvailableMoves;
  UPROPERTY() bool AllowMovement = true;
  UPROPERTY() float MaxWalkSpeed = 400.0f;
  UPROPERTY() float Acceleration = 800.0f;
  UPROPERTY() float Friction = 6.0f;
  UPROPERTY() float TurnRate = 360.0f;
  UPROPERTY() bool CanBlock = true;
  UPROPERTY() float Balance = 100.0f;
  UPROPERTY() bool IsAirborne = false;
  UPROPERTY() bool IsCrouching = false;
};

USTRUCT()
struct FAnimationSet8Way {
  GENERATED_BODY()

  UPROPERTY() FString Idle;
  UPROPERTY() FString WalkForward;
  UPROPERTY() FString WalkBackward;
  UPROPERTY() FString WalkLeft;
  UPROPERTY() FString WalkRight;
  UPROPERTY() FString TurnLeft;
  UPROPERTY() FString TurnRight;
};
