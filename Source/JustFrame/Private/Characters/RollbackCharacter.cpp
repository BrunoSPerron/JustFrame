// Every Frame is a Choice //
#include "Characters/RollbackCharacter.h"
#include "Characters/Stance/StanceInstance.h"
#include "Data/Consts.h"
#include "Data/LogCategories.h"
#include "Data/Structs/InputEnums.h"
#include "Engine/Engine.h"
#include "Serialization/MemoryReader.h"
#include "Serialization/MemoryWriter.h"

ARollbackCharacter::ARollbackCharacter() {
  PrimaryActorTick.bCanEverTick = false;

  // Capsule as Root
  CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
  RootComponent = CapsuleComponent;
  CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  CapsuleComponent->SetSimulatePhysics(false);

  // Skeletal Mesh
  Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
  Mesh->SetupAttachment(CapsuleComponent);
  Mesh->SetRelativeLocation(FVector(0.f, 0.f, -CapsuleComponent->GetUnscaledCapsuleHalfHeight()));
  Mesh->SetSimulatePhysics(false);
  Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

  SetActorEnableCollision(false);
  bReplicates = false;

  SimState.Position = FVector::ZeroVector;
  SimState.Velocity = FVector::ZeroVector;
  SimState.Rotation = FRotator::ZeroRotator;
  SimState.Health = MAX_HEALTH;
}

//Temporary includes
#include "Characters/Stance/StanceInstanceFactory.h"
#include "Characters/Stance/Types/NeutralStance8Way.h"
void ARollbackCharacter::BeginPlay() {
  Super::BeginPlay();

  // TEMP: Default stance setup until real data connection
  FStanceData DefaultStance;
  DefaultStance.StanceId = TEXT("Neutral8Way");
  DefaultStance.AllowMovement = true;
  DefaultStance.MaxWalkSpeed = 400.0f;
  DefaultStance.Acceleration = 1300.0f;
  DefaultStance.Friction = 900.0f;
  DefaultStance.TurnRate = 360.0f;
  CurrentStanceInstance = UStanceInstanceFactory::Create(this, DefaultStance);
  //END TEMP

  SimState.Position = GetActorLocation();
  SimState.Rotation = GetActorRotation();
  SimState.Velocity = FVector::ZeroVector;
  SimState.Health = MAX_HEALTH;
}

void ARollbackCharacter::SimulateFrame(float DeltaTime, const uint16 InputMask) {
  UE_LOG(CharacterLog, VeryVerbose,
         TEXT("ARollbackCharacter::SimulateFrame: [Char %d] InputMask = %u"), GetCharacterIndex(),
         InputMask);
  CurrentStanceInstance->ApplyMovementInput(InputMask, DeltaTime);
}

void ARollbackCharacter::SaveState(TArray<uint8> &OutData) const {
  OutData.SetNumUninitialized(sizeof(FCharacterState));
  FMemory::Memcpy(OutData.GetData(), &SimState, sizeof(FCharacterState));
  UE_LOG(CharacterLog, Verbose, TEXT("SaveState: Saved %d bytes for character %s"), OutData.Num(),
         *GetName());
}

void ARollbackCharacter::LoadState(const TArray<uint8> &InData) {
  check(InData.Num() == sizeof(FCharacterState));
  FMemory::Memcpy(&SimState, InData.GetData(), sizeof(FCharacterState));
  SetActorLocation(SimState.Position);
  UE_LOG(CharacterLog, Verbose,
         TEXT("LoadState: Loaded state for character %s, Position=(%.2f, %.2f, %.2f)"), *GetName(),
         SimState.Position.X, SimState.Position.Y, SimState.Position.Z);
}

void ARollbackCharacter::SetTargetCharacter(ARollbackCharacter *InTarget) { Target = InTarget; }
