// Every Frame is a Choice //
#pragma once

#include "CoreMinimal.h"
#include "Data/Enums_Character.h"
#include "Structs_Move.generated.h"

USTRUCT()
struct FInputCondition {
  GENERATED_BODY()

  UPROPERTY()
  TArray<uint16> Input; // TODO: Sequence of EInputBit codes required to trigger this condition
  UPROPERTY()
  bool bAllowBuffer = true; // TODO: If true, this input can be buffered ahead of the combo window
};

USTRUCT()
struct FComboLink {
  GENERATED_BODY()

  UPROPERTY() FInputCondition InputCondition; // Input requirement for this combo branch
  UPROPERTY() FName MoveID;                   // Move to chain into if condition is met
  UPROPERTY() int32 WindowStart = 0; // TODO: First valid frame (relative to parent move start)
  UPROPERTY() int32 WindowEnd = 0;   // TODO: Last valid frame to perform this link
  UPROPERTY() int8 BufferInput = 0;  // TODO: Max number of frames early the input can be buffered
  UPROPERTY() int8 ChainFrameOffset = 0; // TODO: Offset to delay or blend into the chained move
  UPROPERTY()
  bool bRequireHitConfirm = false; // TODO: Only allow link if current move hit the opponent
};

USTRUCT()
struct FMoveData {
  GENERATED_BODY()

  UPROPERTY() FName MoveID;            // Unique identifier for the move
  UPROPERTY() FName animation;         // TODO: Reference to animation asset
  UPROPERTY() uint8 StartupFrames = 0; // TODO: Number of frames before the move becomes active
  UPROPERTY() int32 ActiveFrames = 0;  // TODO: Duration (in frames) where the move can hit
  UPROPERTY()
  int8 HitRecoveryFrames = 0; // TODO: Frames after active window before returning to neutral
  UPROPERTY()
  int8 WhiffRecoveryFrames = 0; // TODO: Frames after active window before returning to neutral
  UPROPERTY() int8 OnBlockAdvantage = 0; // TODO: Frame advantage/disadvantage when blocked
  UPROPERTY() int8 OnHitAdvantage = 0;   // TODO: Frame advantage/disadvantage when hit connects
  UPROPERTY()
  TArray<FComboLink> ComboLinks; // TODO: Links to other moves if input conditions are met
  UPROPERTY()
  EStanceState ResultingStance =
      EStanceState::Standing; // TODO: Stance to enter after the move ends

  UPROPERTY() TArray<uint8> BalancePerFrame; // TODO: Balance value for each frame
  UPROPERTY()
  int8 CounterWindowModifier = 0; // TODO: Modifies the timing of the counter window during the move
  UPROPERTY() uint8 KnockbackPower = 0;        // TODO: Horizontal pushback on hit
  UPROPERTY() uint8 KnockdownPower = 0;        // TODO: Likelihood of causing knockdown
  UPROPERTY() uint8 LaunchPower = 0;           // TODO: Vertical force applied (for juggles)
  UPROPERTY() uint8 MoveTags;                  // TODO: Bitmask of EMoveTagBit values
  UPROPERTY() TArray<uint16> TrackingPerFrame; // TODO: Tracking strength for each frame
};
