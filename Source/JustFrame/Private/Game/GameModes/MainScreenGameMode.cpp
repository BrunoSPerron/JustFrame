#include "Game/GameModes/MainScreenGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Game/GameManagerSubsystem.h"
#include "UI/CharacterSelectScreen.h"

void AMainScreenGameMode::BeginPlay() {
  Super::BeginPlay();

  GameManagerSubsystem = GetGameInstance()->GetSubsystem<UGameManagerSubsystem>();
  check(GameManagerSubsystem);
  GameManagerSubsystem->SetupCore(2);

  if (!CharacterSelectWidgetClass) {
    UE_LOG(LogTemp, Error,
           TEXT("AMainScreenGameMode::BeginPlay CharacterSelectWidgetClass not assigned in %s"),
           *GetName());
    return;
  }

  if (UWorld *World = GetWorld()) {
    CharacterSelectWidget = CreateWidget<UCharacterSelectScreen>(World, CharacterSelectWidgetClass);
    if (CharacterSelectWidget) {
      CharacterSelectWidget->AddToViewport();
      GameManagerSubsystem->SetGlobalInputMode(EInputMode::Menu);
      CharacterSelectWidget->SetInputActive(true);
    } else {
      UE_LOG(
          LogTemp, Error,
          TEXT("AMainScreenGameMode::BeginPlay Failed to create CharacterSelectWidget instance"));
    }
  }
}
