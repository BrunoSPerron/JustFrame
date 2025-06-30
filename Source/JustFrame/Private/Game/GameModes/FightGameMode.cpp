// Every Frame is a Choice //
#include "Game/GameModes/FightGameMode.h"
#include "Characters/RollbackCharacter.h"
#include "Game/Camera/FightCameraActor.h"
#include "Game/GameManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Systems/Input/InputPollingService.h"
#include "Systems/Player/PlayerSettingsManager.h"
#include "Systems/Rollback/RollbackSimulationManager.h"

AFightGameMode::AFightGameMode() {
  DefaultPawnClass = nullptr;
  PrimaryActorTick.bCanEverTick = false;
}

void AFightGameMode::BeginPlay() {
  Super::BeginPlay();

  // Sync framerate with simulation (Imperfect fix, temporary)
  if (GEngine) GEngine->Exec(nullptr, TEXT("t.MaxFPS 60"));

  ForTestOnly_Autosetup();
  GameManagerSubsystem->SetGlobalInputMode(EInputMode::Fight);

  ResetSimulationTimer(TickRate);
}

void AFightGameMode::PauseSimulation() {
  bSimulationPaused = true;
  UE_LOG(LogTemp, Log, TEXT("AFightGameMode::PauseSimulation Simulation paused."));
}

void AFightGameMode::UnpauseSimulation() {
  bSimulationPaused = false;
  UE_LOG(LogTemp, Log, TEXT("AFightGameMode::UnpauseSimulation Simulation unpaused."));
}

void AFightGameMode::ResetSimulationTimer(float NewRate) {
  if (TickerHandle.IsValid()) {
    FTSTicker::GetCoreTicker().RemoveTicker(TickerHandle);
  }
  TickRate = NewRate;
  float Interval = 1.0f / TickRate;

  TickerHandle = FTSTicker::GetCoreTicker().AddTicker(
      FTickerDelegate::CreateUObject(this, &AFightGameMode::TickSimulation), Interval);

  UE_LOG(LogTemp, Warning, TEXT("Simulation ticker reset to %.2f Hz (interval %.4f)"), TickRate,
         Interval);
}

bool AFightGameMode::TickSimulation(float DeltaTime) {
  if (bSimulationPaused || !GameManagerSubsystem) return true;

  URollbackSimulationManager *SimManager = GameManagerSubsystem->GetRollbackSimManager();
  checkf(SimManager, TEXT("AFightGameMode::TickSimulation: No URollbackSimulationManager found"));
  SimManager->AdvanceSimulation();

  UInputPollingService *InputPollingService = GameManagerSubsystem->GetInputPollingService();
  checkf(InputPollingService,
         TEXT("AFightGameMode::TickSimulation: No UInputPollingService found"));
  InputPollingService->PollControllers();

  return true;
}

void AFightGameMode::ForTestOnly_Autosetup() {
  GameManagerSubsystem = GetGameInstance()->GetSubsystem<UGameManagerSubsystem>();
  check(GameManagerSubsystem);

  // Gather all RollbackCharacters
  TArray<AActor *> FoundActors;
  UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARollbackCharacter::StaticClass(), FoundActors);

  TArray<ARollbackCharacter *> Characters;
  for (AActor *Actor : FoundActors) {
    if (ARollbackCharacter *Character = Cast<ARollbackCharacter>(Actor)) Characters.Add(Character);
  }

  if (Characters.Num() == 0) {
    UE_LOG(LogTemp, Error,
           TEXT("AFightGameMode::ForTestOnly_Autosetup No ARollbackCharacter instances found in "
                "scene."));
    return;
  }

  for (int32 i = 0; i < Characters.Num(); ++i) {
    check(Characters[i]);
    Characters[i]->SetCharacterIndex(i);
  }

  GameManagerSubsystem->SetupCore(Characters.Num());

  // TEMPORARY: map PlayerIDs to character indices in order
  TArray<uint8> PlayerIDs = GameManagerSubsystem->GetPlayerSettingsManager()->GetActivePlayerIDs();
  TMap<uint8, int32> PlayerToCharacter;
  for (int32 i = 0; i < PlayerIDs.Num() && i < Characters.Num(); ++i) {
    PlayerToCharacter.Add(PlayerIDs[i], i);
  }
  GameManagerSubsystem->SetupFight(Characters, PlayerToCharacter);

  if (Characters.Num() == 2) {
    Characters[0]->SetTargetCharacter(Characters[1]);
    Characters[1]->SetTargetCharacter(Characters[0]);
    UWorld *World = GetWorld();
    AFightCameraActor *Cam = World->SpawnActor<AFightCameraActor>(AFightCameraActor::StaticClass());
    Cam->Init(Characters[0], Characters[1]);
    APlayerController *PC = World->GetFirstPlayerController();
    PC->SetViewTarget(Cam);
  }

  UE_LOG(LogTemp, Log,
         TEXT("AFightGameMode::ForTestOnly_Autosetup GameManagerSubsystem initialized with %d "
              "characters."),
         Characters.Num());
}
