// Every Frame is a Choice //
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/Structs_Move.h"
#include "Data/Enums_Character.h"
#include "MoveInstance.h"
#include "CharacterStateMachineComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UCharacterStateMachineComponent : public UActorComponent {
  GENERATED_BODY()

public:
  EStanceState CurrentStance = EStanceState::Standing;
  FMoveInstance CurrentMove;

  void StartMove(const FMoveData *MoveData);
  void TickMove();
  bool IsInMove() const { return CurrentMove.Move != nullptr; }
};
