// Every Frame is a Choice //
#pragma once

#include "Characters/State/Stance/StanceInstance.h"
#include "CoreMinimal.h"
#include "Data/Structs/Structs_Character.h"
#include "UObject/Object.h"
#include "StanceInstanceFactory.generated.h"

class ARollbackCharacter;

UCLASS()
class UStanceInstanceFactory : public UObject {
  GENERATED_BODY()

public:
  static TSharedPtr<FStanceInstance> Create(ARollbackCharacter *Owner,
                                            const FStanceData &StanceData);

  static void
  Register(const FString &StanceId,
           TFunction<TSharedPtr<FStanceInstance>(ARollbackCharacter *, const FStanceData &)>
               Constructor);

private:
  static TMap<FString,
              TFunction<TSharedPtr<FStanceInstance>(ARollbackCharacter *, const FStanceData &)>> &
  GetRegistry();
};
