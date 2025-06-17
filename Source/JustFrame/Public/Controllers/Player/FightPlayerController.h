// Every Frame is a Choice //
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FightPlayerController.generated.h"

UCLASS()
class JUSTFRAME_API AFightPlayerController : public APlayerController {
  GENERATED_BODY()

protected:
  virtual void BeginPlay() override;
};