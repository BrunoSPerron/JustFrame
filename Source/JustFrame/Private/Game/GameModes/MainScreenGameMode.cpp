#include "Game/GameModes/MainScreenGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Game/GameManagerSubsystem.h"
#include "UI/Menu/Screens/CharacterSelectScreen.h"
#include "UI/Menu/MenuManagerComponent.h"
#include "Systems/Input/InputPollingService.h"

AMainScreenGameMode::AMainScreenGameMode() {
  PrimaryActorTick.bCanEverTick = true;
  PrimaryActorTick.bStartWithTickEnabled = true;
}

void AMainScreenGameMode::BeginPlay() {
  Super::BeginPlay();

  UE_LOG(LogTemp, Log, TEXT("AMainScreenGameMode::BeginPlay() called"));

  GameManagerSubsystem = GetGameInstance()->GetSubsystem<UGameManagerSubsystem>();
  check(GameManagerSubsystem);
  GameManagerSubsystem->SetupCore(2);

  if (!MenuManager) {
    MenuManager = NewObject<UMenuManagerComponent>(this, UMenuManagerComponent::StaticClass(),
                                                   TEXT("MenuManagerComponent"));
    MenuManager->RegisterComponent();
  }

  // Temporary for testing: wait a bit for controllers to be detected
  FTimerHandle TimerHandle;
  GetWorld()->GetTimerManager().SetTimer(
      TimerHandle,
      [this]() {
        if (MenuManager && GameManagerSubsystem) {
          UE_LOG(LogTemp, Log, TEXT("AMainScreenGameMode::BeginPlay() attempting to open menu"));
          UPlayerSettingsManager *PlayerSettings = GameManagerSubsystem->GetPlayerSettingsManager();
          if (PlayerSettings) {
            TArray<uint8> PlayerIDs = PlayerSettings->GetActivePlayerIDs();
            UE_LOG(LogTemp, Log, TEXT("AMainScreenGameMode::BeginPlay() found %d active players"),
                   PlayerIDs.Num());
            std::set<uint8> SharedPlayers(PlayerIDs.begin(), PlayerIDs.end());
            if (!SharedPlayers.empty()) {
              uint8 Initiator = *SharedPlayers.begin();
              UE_LOG(LogTemp, Log,
                     TEXT("AMainScreenGameMode::BeginPlay() opening menu for initiator %d"),
                     Initiator);
              MenuManager->OpenMenuForPlayer(Initiator, TEXT("CharacterSelect"), true,
                                             SharedPlayers);
            }
          }
        }
      },
      0.5f, false);
}

void AMainScreenGameMode::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);

  if (GameManagerSubsystem && MenuManager) {
    UInputPollingService *InputPollingService = GameManagerSubsystem->GetInputPollingService();
    if (InputPollingService) {
      InputPollingService->PollControllers();
      MenuManager->TickMenu();
    } else {
      UE_LOG(LogTemp, Log, TEXT("AMainScreenGameMode::Tick no input polling service found"));
    }
  }
}
