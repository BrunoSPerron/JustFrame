#include "Characters/State/Stance/StanceInstance.h"
#include "Game/GameManagerSubsystem.h"
#include "Systems/Rollback/RollbackSimulationManager.h"

const FCharacterState FStanceInstance::GetTargetSimData() const {
  checkf(Owner, TEXT("FStanceInstance::GetTargetSimData: Owner is null."));

  const UGameInstance *GameInstance = Owner->GetGameInstance();
  checkf(GameInstance, TEXT("FStanceInstance::GetTargetSimData: GameInstance is null."));

  UGameManagerSubsystem *GameManager = GameInstance->GetSubsystem<UGameManagerSubsystem>();
  checkf(GameManager, TEXT("FStanceInstance::GetTargetSimData: GameManagerSubsystem not found."));

  URollbackSimulationManager *RollbackSim = GameManager->GetRollbackSimManager();
  checkf(RollbackSim,
         TEXT("FStanceInstance::GetTargetSimData: RollbackSimulationManager is null."));

  int32 CharacterIndex = Owner->GetTargetCharacterIndex();
  checkf(CharacterIndex >= 0, TEXT("FStanceInstance::GetTargetSimData: Invalid target index (%d)."),
         CharacterIndex);

  return RollbackSim->GetCharacterStateAtFrame(RollbackSim->GetCurrentFrame() - 1, CharacterIndex);
}
