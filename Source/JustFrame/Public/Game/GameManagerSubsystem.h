// Every Frame is a Choice //
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameManagerSubsystem.generated.h"

class ARollbackCharacter;
class UInputBufferManager;
class UInputPollingService;
class UPlayerSettingsManager;
class URollbackSimulationManager;
class UCharacterDataDatabase;

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
  UCharacterDataDatabase *GetCharacterDataDatabase() const { return CharacterDataDatabase; }

  void SetupManagers(uint8 NumPlayers, const TArray<ARollbackCharacter *> &Characters);

private:
  UPROPERTY()
  UInputBufferManager *InputBufferManager;

  UPROPERTY()
  UInputPollingService *InputPollingService;

  UPROPERTY()
  UPlayerSettingsManager *PlayerSettingsManager;

  UPROPERTY()
  URollbackSimulationManager *RollbackSimManager;

  UPROPERTY()
  UCharacterDataDatabase *CharacterDataDatabase;
};
