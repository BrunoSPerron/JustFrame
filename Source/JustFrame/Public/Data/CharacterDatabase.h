// Every Frame is a Choice //
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Data/Structs/CharacterStructs.h"
#include "CharacterDatabase.generated.h"

UCLASS()
class JUSTFRAME_API UCharacterDatabase : public UObject {
  GENERATED_BODY()

public:
  void LoadCharacterData(const TArray<FString> &CharacterCollections);

  // Move access
  const FMoveData *FindMove(FName MoveID) const;
  const FMoveData *FindMoveByIndex(uint16 Index) const;
  uint16 GetMoveIndex(FName MoveID) const;

  // Stance access
  const FStanceData *FindStance(FName StanceId) const;
  const FStanceData *FindStanceByIndex(uint16 Index) const;
  uint16 GetStanceIndex(FName StanceId) const;

private:
  void BuildMoveTables(const TArray<FMoveData> &Moves);
  void BuildStanceTables(const TArray<FStanceData> &Stances);

private:
  // Moves
  TMap<FName, FMoveData> MoveMap;
  TArray<FMoveData> MoveList;
  TMap<FName, uint16> MoveIdToIndex;

  // Stances
  TMap<FName, FStanceData> StanceMap;
  TArray<FStanceData> StanceList;
  TMap<FName, uint16> StanceIdToIndex;
};
