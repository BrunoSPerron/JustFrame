// Every Frame is a Choice //
#include "Game/Modes/FightGameMode.h"
#include "Characters/Base/RollbackCharacter.h"
#include "Controllers/Player/FightPlayerController.h"
#include "Game/Camera/FightCameraActor.h"
#include "Game/GameManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Systems/Input/InputPollingService.h"
#include "Systems/Rollback/RollbackSimulationManager.h"

AFightGameMode::AFightGameMode() {
  DefaultPawnClass = nullptr;
  PrimaryActorTick.bCanEverTick = false;
}

void AFightGameMode::BeginPlay() {
  Super::BeginPlay();

  // Ensure Local player exists
  if (GetGameInstance()->GetFirstLocalPlayerController() == nullptr) {
    FString Error;
    GetGameInstance()->CreateLocalPlayer(0, Error, true);
  }

  ForTestOnly_Autosetup();
  ResetSimulationTimer(TickRate);
}

void AFightGameMode::PauseSimulation() {
  // TODO Pause/Unpause GameManagerSubsystem->GetInputPollingService()->THREAD
  bSimulationPaused = true;
  UE_LOG(LogTemp, Log, TEXT("Simulation paused."));
}

void AFightGameMode::UnpauseSimulation() {
  bSimulationPaused = false;
  UE_LOG(LogTemp, Log, TEXT("Simulation unpaused."));
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

  if (URollbackSimulationManager *SimManager = GameManagerSubsystem->GetRollbackSimManager()) {
    SimManager->AdvanceSimulation();
  } else {
    UE_LOG(LogTemp, Log, TEXT("No URollbackSimulationManager instances found in scene"));
  }

  if (UInputPollingService *InputPollingService = GameManagerSubsystem->GetInputPollingService()) {
    InputPollingService->PollControllers();
  } else {
    UE_LOG(LogTemp, Log, TEXT("No UInputPollingService instances found in scene"));
  }

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
    UE_LOG(LogTemp, Error, TEXT("No ARollbackCharacter instances found in scene."));
    return;
  }

  GameManagerSubsystem->SetupManagers(Characters.Num(), Characters);

  if (Characters.Num() == 2) {
    UWorld *World = GetWorld();
    AFightCameraActor *Cam = World->SpawnActor<AFightCameraActor>(AFightCameraActor::StaticClass());
    Cam->Init(Characters[0], Characters[1]);
    APlayerController *PC = World->GetFirstPlayerController();
    PC->SetViewTarget(Cam);
  }

  UE_LOG(LogTemp, Log, TEXT("GameManagerSubsystem initialized with %d characters."),
         Characters.Num());
}
