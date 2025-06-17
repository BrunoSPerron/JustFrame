// Every Frame is a Choice //
#pragma once

#include "CoreMinimal.h"
#include "IMoveDataSource.h"
#include "MoveDatabase.generated.h"

UCLASS()
class UMoveDatabase : public UObject {
  GENERATED_BODY()

public:
  void LoadMoves(const TArray<FString> &Collections);
  const FMoveData *FindMove(FName MoveID) const;

private:
  TMap<FName, FMoveData> MoveMap;
  TUniquePtr<IMoveDataSource> DataSource;
};
