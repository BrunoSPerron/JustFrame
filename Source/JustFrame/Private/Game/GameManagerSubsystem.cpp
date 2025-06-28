// Every Frame is a Choice //
#include "Game/GameManagerSubsystem.h"
#include "Data/CharacterDatabase.h"
#include "Data/LogCategories.h"
#include "Systems/Input/InputBufferManager.h"
#include "Systems/Input/InputPollingService.h"
#include "Systems/Input/InputRouter.h"
#include "Systems/Input/Consumers/FightInputConsumer.h"
#include "Systems/Input/Consumers/MenuInputConsumer.h"
#include "Systems/Player/PlayerSettingsManager.h"
#include "Systems/Rollback/RollbackSimulationManager.h"
#include "Engine/GameInstance.h"

void UGameManagerSubsystem::Initialize(FSubsystemCollectionBase &Collection) {
  Super::Initialize(Collection);
}

void UGameManagerSubsystem::Deinitialize() {
  Super::Deinitialize();
  InputBufferManager = nullptr;
  InputPollingService = nullptr;
  PlayerSettingsManager = nullptr;
  RollbackSimManager = nullptr;
  CharacterDatabase = nullptr;
  InputRouter.Reset();
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

  if (!CharacterDatabase) {
    CharacterDatabase = NewObject<UCharacterDatabase>();
    TArray<FString> CharacterCollections = {TEXT("Manny")};
    CharacterDatabase->LoadCharacterData(CharacterCollections);
  }

  if (!InputRouter) {
    InputRouter = MakeUnique<FInputRouter>(PlayerSettingsManager);
    InputRouter->RegisterConsumer(EInputMode::Menu, MakeShared<FMenuInputConsumer>());
    InputRouter->RegisterConsumer(
        EInputMode::Fight,
        MakeShared<FFightInputConsumer>(InputBufferManager, PlayerSettingsManager));
  }
  InputPollingService->SetInputRouter(InputRouter.Get());
}

void UGameManagerSubsystem::SetInputMode(uint8 PlayerID, EInputMode NewMode) {
  if (InputRouter) {
    InputRouter->SetInputMode(PlayerID, NewMode);
  } else {
    UE_LOG(InputLog, Warning,
           TEXT("UGameManagerSubsystem::SetInputMode InputRouter is null (PlayerID: %d, Mode: %d)"),
           PlayerID, static_cast<uint8>(NewMode));
  }
}

void UGameManagerSubsystem::SetGlobalInputMode(EInputMode NewMode) {
  if (InputRouter) {
    InputRouter->SetGlobalInputMode(NewMode);
  } else {
    checkf(InputRouter != nullptr,
           TEXT("UGameManagerSubsystem::SetGlobalInputMode called before InputRouter was "
                "initialized"));
  }
}
