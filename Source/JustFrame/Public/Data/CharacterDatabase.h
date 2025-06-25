// Every Frame is a Choice //
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Data/Structs/Structs_Character.h"
#include "Data/Sources/IMoveDataSource.h"
#include "CharacterDatabase.generated.h"

class IMoveDataSource;

UCLASS()
class UCharacterDatabase : public UObject {
  GENERATED_BODY()

public:
  void LoadCharacterData(const TArray<FString> &CharacterCollections);

  const FMoveData *FindMove(FName MoveID) const;
  const FMoveData *FindMoveByIndex(uint16 Index) const;
  uint16 GetMoveIndex(FName MoveID) const;

  const FStanceData *FindStance(FString StanceId) const;
  const FStanceData *FindStanceByIndex(uint16 Index) const;
  uint16 GetStanceIndex(FString StanceId) const;

private:
  TUniquePtr<IMoveDataSource> DataSource;

  TMap<FName, FMoveData> MoveMap;
  TArray<FMoveData> MoveList;
  TMap<FName, uint16> MoveIdToIndex;

  TMap<FString, FStanceData> StanceMap;
  TArray<FStanceData> StanceList;
  TMap<FString, uint16> StanceIdToIndex;
};
