// Every Frame is a Choice //
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainScreenGameMode.generated.h"

class UGameManagerSubsystem;
class UMenuManagerComponent;

UCLASS() class JUSTFRAME_API AMainScreenGameMode : public AGameModeBase {
  GENERATED_BODY()

public:
  AMainScreenGameMode();

protected:
  virtual void BeginPlay() override;
  virtual void Tick(float DeltaTime) override;

private:
  UPROPERTY(EditDefaultsOnly, Category = "UI")
  TSubclassOf<class UCharacterSelectScreen> CharacterSelectWidgetClass;

  UPROPERTY()
  UMenuManagerComponent *MenuManager = nullptr;

  UGameManagerSubsystem *GameManagerSubsystem;
};
