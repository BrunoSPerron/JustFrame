// Every Frame is a Choice //
#pragma once

#include "CoreMinimal.h"
#include "Containers/Ticker.h"
#include "Data/Consts.h"
#include "GameFramework/GameModeBase.h"
#include "FightGameMode.generated.h"

class UGameManagerSubsystem;

UCLASS()
class JUSTFRAME_API AFightGameMode : public AGameModeBase {
  GENERATED_BODY()

public:
  AFightGameMode();

  virtual void BeginPlay() override;

  UFUNCTION()
  void PauseSimulation();

  UFUNCTION()
  void UnpauseSimulation();

  UFUNCTION()
  void ResetSimulationTimer(float NewRate);

private:
  bool TickSimulation(float DeltaTime);
  void ForTestOnly_Autosetup();

  UGameManagerSubsystem *GameManagerSubsystem;

  FTSTicker::FDelegateHandle TickerHandle;
  float TickRate = 60.f;
  bool bSimulationPaused = false;
};
