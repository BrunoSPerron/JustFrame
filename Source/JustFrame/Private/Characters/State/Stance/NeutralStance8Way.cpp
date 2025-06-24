// Every Frame is a Choice //
#include "Characters/State/Stance/NeutralStance8Way.h"
#include "Characters/State/Stance/StanceRegistration.h"
#include "Characters/Base/RollbackCharacter.h"
#include "Data/Structs/Enums_Input.h"
#include "Data/LogCategories.h"

FNeutralStance8Way::FNeutralStance8Way(ARollbackCharacter *InOwner, const FStanceData &InData)
    : FStanceInstance(InOwner, InData) {}

void FNeutralStance8Way::ApplyMovementInput(uint16 InputMask, float DeltaTime) {
  FCharacterState *SimState = Owner->GetSimState();
  const FCharacterState TargetState = GetTargetSimData();

  const FVector ToTarget = (TargetState.Position - SimState->Position).GetSafeNormal2D();
  const FVector RightVec = FVector::CrossProduct(FVector::UpVector, ToTarget);

  FVector MoveInput = FVector::ZeroVector;
  if (StanceData.AllowMovement) {
    if (InputMask & static_cast<uint16>(EInputBit::Right)) MoveInput += ToTarget;
    if (InputMask & static_cast<uint16>(EInputBit::Left)) MoveInput -= ToTarget;
    if (InputMask & static_cast<uint16>(EInputBit::Down)) MoveInput += RightVec;
    if (InputMask & static_cast<uint16>(EInputBit::Up)) MoveInput -= RightVec;
  }

  const FVector MoveDir = MoveInput.GetSafeNormal();
  const float MaxSpeed = static_cast<float>(StanceData.MaxWalkSpeed);
  const float Accel = static_cast<float>(StanceData.Acceleration);
  const float Friction = static_cast<float>(StanceData.Friction);

  if (!MoveDir.IsNearlyZero()) {
    const FVector DesiredVelocity = MoveDir * MaxSpeed;
    const FVector VelocityDelta = DesiredVelocity - SimState->Velocity;
    const FVector AccelStep = VelocityDelta.GetClampedToMaxSize(Accel * DeltaTime);
    SimState->Velocity += AccelStep;

    UE_LOG(StanceLog, VeryVerbose,
           TEXT("FNeutralStance8Way::ApplyMovementInput: [Char %d] Applying input -> MoveDir: "
                "(%.2f, %.2f), AccelStep: (%.2f, %.2f)"),
           Owner->GetCharacterIndex(), MoveDir.X, MoveDir.Y, AccelStep.X, AccelStep.Y);
  } else {
    const float Speed = SimState->Velocity.Size();
    const float FrictionDrop = Friction * DeltaTime;
    const float NewSpeed = FMath::Max(Speed - FrictionDrop, 0.f);
    SimState->Velocity = SimState->Velocity.GetSafeNormal() * NewSpeed;

    UE_LOG(StanceLog, VeryVerbose,
           TEXT("FNeutralStance8Way::ApplyMovementInput: [Char %d] No input -> Friction drop to "
                "speed %.2f"),
           Owner->GetCharacterIndex(), NewSpeed);
  }

  if (SimState->Velocity.SizeSquared() > MaxSpeed * MaxSpeed) {
    SimState->Velocity = SimState->Velocity.GetClampedToMaxSize(MaxSpeed);
    UE_LOG(StanceLog, VeryVerbose,
           TEXT("FNeutralStance8Way::ApplyMovementInput: [Char %d] Velocity clamped to MaxSpeed "
                "(%.2f)"),
           Owner->GetCharacterIndex(), MaxSpeed);
  }

  const FVector PrevPosition = SimState->Position;
  SimState->Position += SimState->Velocity * DeltaTime;
  Owner->SetActorLocation(SimState->Position);

  const FRotator TargetYaw = ToTarget.Rotation();
  const FRotator DesiredRot = FRotator(0.f, TargetYaw.Yaw - 90.f, 0.f);
  const FRotator NewRot = FMath::RInterpConstantTo(SimState->Rotation, DesiredRot, DeltaTime,
                                                   static_cast<float>(StanceData.TurnRate));
  SimState->Rotation = NewRot;
  Owner->SetActorRotation(NewRot);

  UE_LOG(StanceLog, Verbose,
         TEXT("FNeutralStance8Way::ApplyMovementInput: [Char %d] Position updated (%.2f, %.2f) → "
              "(%.2f, %.2f), Velocity: (%.2f, %.2f)"),
         Owner->GetCharacterIndex(), PrevPosition.X, PrevPosition.Y, SimState->Position.X,
         SimState->Position.Y, SimState->Velocity.X, SimState->Velocity.Y);
}

REGISTER_STANCE_INSTANCE(FNeutralStance8Way, TEXT("Neutral8Way"));
