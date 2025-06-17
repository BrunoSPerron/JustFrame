// Every Frame is a Choice //
#include "Game/Camera/FightCameraActor.h"
#include "Kismet/GameplayStatics.h"

AFightCameraActor::AFightCameraActor() {
  PrimaryActorTick.bCanEverTick = true;
  PrimaryActorTick.bStartWithTickEnabled = true;
}

void AFightCameraActor::Init(AActor *Player1, AActor *Player2) {
  TrackedFighter1 = Player1;
  TrackedFighter2 = Player2;
}

void AFightCameraActor::BeginPlay() { Super::BeginPlay(); }

void AFightCameraActor::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);

  if (!TrackedFighter1 || !TrackedFighter2) return;

  // Get midpoint between fighters
  const FVector Pos1 = TrackedFighter1->GetActorLocation();
  const FVector Pos2 = TrackedFighter2->GetActorLocation();
  const FVector Midpoint = (Pos1 + Pos2) * 0.5f;

  // Get direction between fighters and perpendicular offset vector
  const FVector2D Direction2D = FVector2D(Pos2 - Pos1).GetSafeNormal();
  FVector2D Perpendicular2D = FVector2D(-Direction2D.Y, Direction2D.X);

  FVector Offset3D = FVector(Perpendicular2D, 0.f) * 250 + FVector(0, 0, 40);
  TargetPosition = Midpoint + Offset3D;

  // Lerp toward position
  const FVector CurrentPos = GetActorLocation();
  const FVector NewPos =
      FMath::VInterpTo(CurrentPos, TargetPosition, DeltaTime, 5.f); // 5 is the interp speed
  SetActorLocation(NewPos);

  // Look at the midpoint
  const FRotator DesiredRotation = (Midpoint - NewPos).Rotation();
  TargetRotation = DesiredRotation;

  // Lerp toward rotation
  const FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, 5.f);
  SetActorRotation(NewRotation);
}
