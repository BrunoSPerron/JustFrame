// Every Frame is a Choice //
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainScreenGameMode.generated.h"

class UCharacterSelectScreen;
class UGameManagerSubsystem;

UCLASS() class JUSTFRAME_API AMainScreenGameMode : public AGameModeBase {
  GENERATED_BODY()

protected:
  virtual void BeginPlay() override;

private:
  UPROPERTY()
  UCharacterSelectScreen *CharacterSelectWidget = nullptr;

  UPROPERTY(EditDefaultsOnly, Category = "UI")
  TSubclassOf<UCharacterSelectScreen> CharacterSelectWidgetClass;

  UGameManagerSubsystem *GameManagerSubsystem;
};
