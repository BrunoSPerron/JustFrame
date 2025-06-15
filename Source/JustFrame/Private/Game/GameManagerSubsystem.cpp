// Every Frame is a Choice
#include "Game/GameManagerSubsystem.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Systems/Input/InputBufferManager.h"
#include "Systems/Input/InputPollingService.h"
#include "Systems/Player/PlayerSettingsManager.h"
#include "Systems/Rollback/RollbackSimulationManager.h"

void UGameManagerSubsystem::Initialize(FSubsystemCollectionBase &Collection) {
  Super::Initialize(Collection);
}

void UGameManagerSubsystem::Deinitialize() {
  Super::Deinitialize();
  InputBufferManager = nullptr;
  InputPollingService = nullptr;
  PlayerSettingsManager = nullptr;
  RollbackSimManager = nullptr;
}

void UGameManagerSubsystem::SetupManagers(uint8 NumPlayers,
                                          const TArray<ARollbackCharacter *> &Characters) {
  UGameInstance *GameInstance = GetGameInstance();
  if (!InputBufferManager) {
    InputBufferManager = NewObject<UInputBufferManager>(this);
    InputBufferManager->Init(NumPlayers);
  }
  if (!PlayerSettingsManager) {
    PlayerSettingsManager = NewObject<UPlayerSettingsManager>(this);
    PlayerSettingsManager->Init(NumPlayers);
  }
  if (!RollbackSimManager) {
    RollbackSimManager = NewObject<URollbackSimulationManager>(this);
    RollbackSimManager->Init(Characters, InputBufferManager);
  }
  if (!InputPollingService) {
    InputPollingService = NewObject<UInputPollingService>(this);
    InputPollingService->Init(InputBufferManager, PlayerSettingsManager, RollbackSimManager);
  }
}
