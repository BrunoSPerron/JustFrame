// Every Frame is a Choice //
#include "Characters/State/CharacterStateMachineComponent.h"

void UCharacterStateMachineComponent::StartMove(const FMoveData *MoveData) {
  if (!MoveData) return;
  CurrentMove = FMoveInstance{MoveData};
}

void UCharacterStateMachineComponent::TickMove() {
  if (!IsInMove()) return;

  CurrentMove.Tick();
  if (CurrentMove.IsDone()) {
    CurrentStance = CurrentMove.Move->ResultingStance;
    CurrentMove = FMoveInstance{};
  }
}
