#include "UI/Interfaces/IMenuInputCore.h"
#include "Game/GameManagerSubsystem.h"
#include "Systems/Input/InputBufferManager.h"
#include "Systems/Player/PlayerSettingsManager.h"
#include "Systems/Input/InputPollingService.h"

void IMenuInputCore::TriggerInputs(UObject *ContextObject) {
  if (!bIsActive) return;
  if (!ContextObject) {
    UE_LOG(LogTemp, Warning, TEXT("IMenuInputCore::TriggerInputs: ContextObject is null."));
    return;
  }
  UWorld *World = ContextObject->GetWorld();
  if (!IsValid(World)) {
    UE_LOG(LogTemp, Warning, TEXT("IMenuInputCore::TriggerInputs: World is invalid."));
    return;
  }
  UGameInstance *GameInstance = World->GetGameInstance();
  if (!IsValid(GameInstance)) {
    UE_LOG(LogTemp, Warning, TEXT("IMenuInputCore::TriggerInputs: GameInstance is invalid."));
    return;
  }
  UGameManagerSubsystem *GameManager = GameInstance->GetSubsystem<UGameManagerSubsystem>();
  if (!IsValid(GameManager)) {
    UE_LOG(LogTemp, Warning,
           TEXT("IMenuInputCore::TriggerInputs: GameManagerSubsystem is invalid."));
    return;
  }
  UInputPollingService *PollingService = GameManager->GetInputPollingService();
  if (!IsValid(PollingService)) {
    UE_LOG(LogTemp, Warning,
           TEXT("IMenuInputCore::TriggerInputs: InputPollingService is invalid."));
    return;
  }
  PollingService->PollControllers();
  UInputBufferManager *Buffer = GameManager->GetInputBufferManager();
  if (!IsValid(Buffer)) {
    UE_LOG(LogTemp, Warning, TEXT("IMenuInputCore::TriggerInputs: InputBufferManager is invalid."));
    return;
  }
  UPlayerSettingsManager *PlayerSettingsManager = GameManager->GetPlayerSettingsManager();
  if (!IsValid(PlayerSettingsManager)) {
    UE_LOG(LogTemp, Warning,
           TEXT("IMenuInputCore::TriggerInputs: PlayerSettingsManager is invalid."));
    return;
  }
  TArray<uint8> PlayerIDs = PlayerSettingsManager->GetActivePlayerIDs();

  for (uint8 PlayerID : PlayerIDs) {
    const uint16 InputMask = Buffer->GetUnbufferedInput(PlayerID);
    if (InputMask != 0) {
      ProcessInput(PlayerID, InputMask);
    }
  }
}