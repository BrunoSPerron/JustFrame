// Every Frame is a Choice
#include "Characters/Base/RollbackCharacter.h"
#include "Data/Consts.h"
#include "Data/Enums_Input.h"
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

  // We're driving movement manually
  SetActorEnableCollision(false);
  bReplicates = false;
}

void ARollbackCharacter::BeginPlay() {
  Super::BeginPlay();

  // Set initial state based on actor transform
  SimState.Position = GetActorLocation();
  SimState.Velocity = FVector::ZeroVector;
  SimState.Health = MAX_HEALTH;
}

void ARollbackCharacter::SimulateFrame(float DeltaTime, const uint16 InputMask) {
  ApplyInput(InputMask, DeltaTime);
  UpdateMovement(DeltaTime);
  SetActorLocation(SimState.Position);
}

void ARollbackCharacter::ApplyInput(const uint16 InputMask, float DeltaTime) {
  // TODO replace with state machine
  FVector InputVec = FVector::ZeroVector;
  if (InputMask & static_cast<uint16>(EInputBit::Right)) InputVec.X += 1.f;
  if (InputMask & static_cast<uint16>(EInputBit::Left)) InputVec.X -= 1.f;
  if (InputMask & static_cast<uint16>(EInputBit::Up)) InputVec.Y += 1.f;
  if (InputMask & static_cast<uint16>(EInputBit::Down)) InputVec.Y -= 1.f;
  InputVec = InputVec.GetClampedToMaxSize(1.0f);
  SimState.Velocity = InputVec * 600.f;
}

void ARollbackCharacter::UpdateMovement(float DeltaTime) {
  SimState.Position += SimState.Velocity * DeltaTime;
}

void ARollbackCharacter::SaveState(TArray<uint8> &OutData) const {
  OutData.SetNumUninitialized(sizeof(FCharacterState));
  FMemory::Memcpy(OutData.GetData(), &SimState, sizeof(FCharacterState));
}

void ARollbackCharacter::LoadState(const TArray<uint8> &InData) {
  check(InData.Num() == sizeof(FCharacterState));
  FMemory::Memcpy(&SimState, InData.GetData(), sizeof(FCharacterState));
  SetActorLocation(SimState.Position);
}
