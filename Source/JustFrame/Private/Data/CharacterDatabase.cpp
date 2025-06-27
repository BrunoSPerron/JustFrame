// Every Frame is a Choice //
#include "Data/CharacterDatabase.h"
#include "Utilities/ExternalDataLoader.h"
#include "Data/LogCategories.h"
#include "Data/Consts.h"

void UCharacterDatabase::LoadCharacterData(const TArray<FString> &CharacterCollections) {
  TArray<FMoveData> LoadedMoves;
  TArray<FStanceData> LoadedStances;

  if (!ExternalDataLoader::LoadCharacterData(CharacterCollections, LoadedMoves, LoadedStances)) {
    UE_LOG(
        CharacterDBLog, Error,
        TEXT(
            "UCharacterDatabase::LoadCharacterData Failed to load or parse character data files."));
    return;
  }

  BuildMoveTables(LoadedMoves);
  BuildStanceTables(LoadedStances);

  UE_LOG(CharacterDBLog, Log,
         TEXT("UCharacterDatabase::LoadCharacterData Loaded %d moves, %d stances"), MoveList.Num(),
         StanceList.Num());
}

void UCharacterDatabase::BuildMoveTables(const TArray<FMoveData> &Moves) {
  MoveMap.Empty();
  MoveList.Empty();
  MoveIdToIndex.Empty();

  for (int32 i = 0; i < Moves.Num(); ++i) {
    const FMoveData &Move = Moves[i];
    MoveMap.Add(Move.MoveID, Move);
    MoveIdToIndex.Add(Move.MoveID, i);
    MoveList.Add(Move);

    UE_LOG(CharacterDBLog, VeryVerbose,
           TEXT("UCharacterDatabase::BuildMoveTables [Move %d] ID=%s | Startup=%d | Recovery=%d | "
                "OnBlock=%d"),
           i, *Move.MoveID.ToString(), Move.StartupFrames, Move.WhiffRecoveryFrames,
           Move.OnBlockAdvantage);
  }
}

void UCharacterDatabase::BuildStanceTables(const TArray<FStanceData> &Stances) {
  StanceMap.Empty();
  StanceList.Empty();
  StanceIdToIndex.Empty();

  for (int32 i = 0; i < Stances.Num(); ++i) {
    const FStanceData &Stance = Stances[i];
    const FName StanceName = FName(Stance.StanceId);
    StanceMap.Add(StanceName, Stance);
    StanceIdToIndex.Add(StanceName, i);
    StanceList.Add(Stance);

    UE_LOG(CharacterDBLog, VeryVerbose,
           TEXT("UCharacterDatabase::BuildStanceTables [Stance %d] ID=%s | Movement=%s"), i,
           *StanceName.ToString(), Stance.AllowMovement ? TEXT("Yes") : TEXT("No"));
  }
}

// Move access
const FMoveData *UCharacterDatabase::FindMove(FName MoveID) const { return MoveMap.Find(MoveID); }

const FMoveData *UCharacterDatabase::FindMoveByIndex(uint16 Index) const {
  return MoveList.IsValidIndex(Index) ? &MoveList[Index] : nullptr;
}

uint16 UCharacterDatabase::GetMoveIndex(FName MoveID) const {
  const uint16 *Index = MoveIdToIndex.Find(MoveID);
  return Index ? *Index : INDEX_NONE;
}

// Stance access (FName-based)
const FStanceData *UCharacterDatabase::FindStance(FName StanceId) const {
  return StanceMap.Find(StanceId);
}

const FStanceData *UCharacterDatabase::FindStanceByIndex(uint16 Index) const {
  return StanceList.IsValidIndex(Index) ? &StanceList[Index] : nullptr;
}

uint16 UCharacterDatabase::GetStanceIndex(FName StanceId) const {
  const uint16 *Index = StanceIdToIndex.Find(StanceId);
  return Index ? *Index : INDEX_NONE;
}
