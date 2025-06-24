// Every Frame is a Choice //
#include "Characters/State/Stance/StanceInstanceFactory.h"

TMap<FString, TFunction<TSharedPtr<FStanceInstance>(ARollbackCharacter *, const FStanceData &)>> &
UStanceInstanceFactory::GetRegistry() {
  static TMap<FString,
              TFunction<TSharedPtr<FStanceInstance>(ARollbackCharacter *, const FStanceData &)>>
      Registry;
  return Registry;
}

void UStanceInstanceFactory::Register(
    const FString &StanceId,
    TFunction<TSharedPtr<FStanceInstance>(ARollbackCharacter *, const FStanceData &)> Constructor) {
  GetRegistry().Add(StanceId, MoveTemp(Constructor));
}

TSharedPtr<FStanceInstance> UStanceInstanceFactory::Create(ARollbackCharacter *Owner,
                                                           const FStanceData &StanceData) {
  const auto *Func = GetRegistry().Find(StanceData.StanceId);
  return Func ? (*Func)(Owner, StanceData) : MakeShared<FStanceInstance>(Owner, StanceData);
}
