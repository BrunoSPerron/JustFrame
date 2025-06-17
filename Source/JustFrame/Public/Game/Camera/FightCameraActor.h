// Every Frame is a Choice //
#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "FightCameraActor.generated.h"

UCLASS()
class JUSTFRAME_API AFightCameraActor : public ACameraActor {
  GENERATED_BODY()

public:
  AFightCameraActor();
  virtual void Tick(float DeltaTime) override;

  void Init(AActor *Player1, AActor *Player2);

protected:
  virtual void BeginPlay() override;

private:
  FVector TargetPosition;
  FRotator TargetRotation;
  AActor *TrackedFighter1;
  AActor *TrackedFighter2;
};
