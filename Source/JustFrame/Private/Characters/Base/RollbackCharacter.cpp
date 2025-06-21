// Every Frame is a Choice //
#include "Characters/Base/RollbackCharacter.h"
#include "Data/Consts.h"
#include "Data/Structs/Enums_Input.h"
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
}

void ARollbackCharacter::BeginPlay() {
  Super::BeginPlay();

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
  if (!target) {
    SimState.Velocity = FVector::ZeroVector;
    return;
  }

  const FVector ToTarget = (target->SimState.Position - SimState.Position).GetSafeNormal2D();
  const FVector RightVec = FVector::CrossProduct(FVector::UpVector, ToTarget);

  FVector MoveInput = FVector::ZeroVector;
  if (InputMask & static_cast<uint16>(EInputBit::Right)) MoveInput += ToTarget; // approach
  if (InputMask & static_cast<uint16>(EInputBit::Left)) MoveInput -= ToTarget;  // retreat
  if (InputMask & static_cast<uint16>(EInputBit::Down)) MoveInput += RightVec;  // strafe right
  if (InputMask & static_cast<uint16>(EInputBit::Up)) MoveInput -= RightVec;    // strafe left
  MoveInput = MoveInput.GetClampedToMaxSize(1.0f);

  const float MaxSpeed = 600.f;
  const float Accel = 3000.f; // units/sec^2
  const FVector DesiredVelocity = MoveInput * MaxSpeed;
  const FVector VelocityDelta = DesiredVelocity - SimState.Velocity;

  const FVector AccelStep = VelocityDelta.GetClampedToMaxSize(Accel * DeltaTime);
  SimState.Velocity += AccelStep;

  const FRotator FaceRot = ToTarget.Rotation();
  const FRotator SimRot = FRotator(0.f, FaceRot.Yaw - 90.f, 0.f);
  SetActorRotation(SimRot);
  SimState.Rotation = SimRot;
}

void ARollbackCharacter::UpdateMovement(float DeltaTime) {
  SimState.Position += SimState.Velocity * DeltaTime;
  if (SimState.Velocity.SizeSquared() < 1.0f) SimState.Velocity = FVector::ZeroVector;
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

const FCharacterState *ARollbackCharacter::GetSimState() { return &SimState; }

void ARollbackCharacter::SetTarget(ARollbackCharacter *Target) { target = Target; }
