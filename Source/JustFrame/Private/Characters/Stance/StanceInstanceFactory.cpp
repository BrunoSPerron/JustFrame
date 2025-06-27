// Every Frame is a Choice //
#include "Characters/Stance/StanceInstanceFactory.h"

TMap<FName, TFunction<TSharedPtr<FStanceInstance>(ARollbackCharacter *, const FStanceData &)>> &
UStanceInstanceFactory::GetRegistry() {
  static TMap<FName,
              TFunction<TSharedPtr<FStanceInstance>(ARollbackCharacter *, const FStanceData &)>>
      Registry;
  return Registry;
}

void UStanceInstanceFactory::Register(
    const FName &StanceId,
    TFunction<TSharedPtr<FStanceInstance>(ARollbackCharacter *, const FStanceData &)> Constructor) {
  GetRegistry().Add(StanceId, MoveTemp(Constructor));
}

TSharedPtr<FStanceInstance> UStanceInstanceFactory::Create(ARollbackCharacter *Owner,
                                                           const FStanceData &StanceData) {
  const auto *Func = GetRegistry().Find(StanceData.StanceId);
  return Func ? (*Func)(Owner, StanceData) : MakeShared<FStanceInstance>(Owner, StanceData);
}
