// Every Frame is a Choice //
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Data/Structs/Structs_Move.h"
#include "Data/Sources/IMoveDataSource.h"
#include "CharacterDataDatabase.generated.h"

UCLASS()
class UCharacterDataDatabase : public UObject {
  GENERATED_BODY()

public:
  void LoadCharacterData(const TArray<FString> &MoveCollections,
                         const TArray<FString> &StanceCollections);

  // Move access
  const FMoveData *FindMove(FName MoveID) const;
  const FMoveData *FindMoveByIndex(uint16 Index) const;
  uint16 GetMoveIndex(FName MoveID) const;

  // Stance access
  const FNeutralStanceData *FindStance(FString StanceId) const;
  const FNeutralStanceData *FindStanceByIndex(uint16 Index) const;
  uint16 GetStanceIndex(FString StanceId) const;

private:
  TUniquePtr<IMoveDataSource> DataSource;

  // Move data
  TArray<FMoveData> MoveList;
  TMap<FName, FMoveData> MoveMap;
  TMap<FName, uint16> MoveIdToIndex;

  // Stance data
  TArray<FNeutralStanceData> StanceList;
  TMap<FString, FNeutralStanceData> StanceMap;
  TMap<FString, uint16> StanceIdToIndex;
};
