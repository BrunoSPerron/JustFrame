// Every Frame is a Choice //
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Systems/Input/InputRouter.h"
#include "GameManagerSubsystem.generated.h"

class ARollbackCharacter;
class UInputBufferManager;
class UInputPollingService;
class UPlayerSettingsManager;
class URollbackSimulationManager;
class UCharacterDatabase;

// Access from anywhere
// GetGameInstance()->GetSubsystem<UGameManagerSubsystem>();

UCLASS()
class JUSTFRAME_API UGameManagerSubsystem : public UGameInstanceSubsystem {
  GENERATED_BODY()

public:
  virtual void Initialize(FSubsystemCollectionBase &Collection) override;
  virtual void Deinitialize() override;

  UInputBufferManager *GetInputBufferManager() const { return InputBufferManager; }
  UInputPollingService *GetInputPollingService() const { return InputPollingService; }
  UPlayerSettingsManager *GetPlayerSettingsManager() const { return PlayerSettingsManager; }
  URollbackSimulationManager *GetRollbackSimManager() const { return RollbackSimManager; }
  UCharacterDatabase *GetCharacterDatabase() const { return CharacterDatabase; }
  FInputRouter *GetInputRouter() const { return InputRouter.Get(); }

  void SetupCore(uint8 NumPlayers);
  void SetupFight(const TArray<ARollbackCharacter *> &Characters,
                  const TMap<uint8, int32> &PlayerToCharacter);
  void SetInputMode(uint8 PlayerID, EInputMode NewMode);
  void SetGlobalInputMode(EInputMode NewMode);

private:
  UPROPERTY()
  UInputBufferManager *InputBufferManager = nullptr;

  UPROPERTY()
  UInputPollingService *InputPollingService = nullptr;

  UPROPERTY()
  UPlayerSettingsManager *PlayerSettingsManager = nullptr;

  UPROPERTY()
  URollbackSimulationManager *RollbackSimManager = nullptr;

  UPROPERTY()
  UCharacterDatabase *CharacterDatabase = nullptr;

  TUniquePtr<FInputRouter> InputRouter;
};
