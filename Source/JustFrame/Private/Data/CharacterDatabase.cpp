// Every Frame is a Choice //
#include "Data/CharacterDatabase.h"
#include "Data/Sources/JsonFileListMoveDataSource.h"
#include "Utilities/CryptoUtils.h"
#include "Data/LogCategories.h"
#include "Data/Consts.h"

void UCharacterDatabase::LoadCharacterData(const TArray<FString> &CharacterCollections) {
  DataSource = MakeUnique<FJsonFileListMoveDataSource>();

  TArray<FMoveData> LoadedMoves;
  TArray<FStanceData> LoadedStances;
  if (!DataSource->LoadCharacterSources(CharacterCollections, LoadedMoves, LoadedStances)) {
    UE_LOG(
        MoveDBLog, Error,
        TEXT("UCharacterDatabase::LoadCharacterData Failed to load or parse character sources."));
    return;
  }

  BuildMoveTables(LoadedMoves);
  BuildStanceTables(LoadedStances);

  UE_LOG(MoveDBLog, Log,
         TEXT("UCharacterDatabase::LoadCharacterData Character data loaded: %d moves, %d stances"),
         MoveList.Num(), StanceList.Num());
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

    UE_LOG(
        MoveDBLog, VeryVerbose,
        TEXT("UCharacterDatabase::LogMove [Move %d] ID=%s | Startup=%d | Recovery=%d | OnBlock=%d"),
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
    StanceMap.Add(Stance.StanceId, Stance);
    StanceIdToIndex.Add(Stance.StanceId, i);
    StanceList.Add(Stance);

    UE_LOG(MoveDBLog, VeryVerbose,
           TEXT("UCharacterDatabase::LogStance [Stance %d] ID=%s | Movement=%s"), i,
           *Stance.StanceId, Stance.AllowMovement ? TEXT("Yes") : TEXT("No"));
  }
}

// Move getters
const FMoveData *UCharacterDatabase::FindMove(FName MoveID) const { return MoveMap.Find(MoveID); }
const FMoveData *UCharacterDatabase::FindMoveByIndex(uint16 Index) const {
  return MoveList.IsValidIndex(Index) ? &MoveList[Index] : nullptr;
}
uint16 UCharacterDatabase::GetMoveIndex(FName MoveID) const {
  const uint16 *Index = MoveIdToIndex.Find(MoveID);
  return Index ? *Index : INDEX_NONE;
}

// Stance getters
const FStanceData *UCharacterDatabase::FindStance(FString StanceId) const {
  return StanceMap.Find(StanceId);
}
const FStanceData *UCharacterDatabase::FindStanceByIndex(uint16 Index) const {
  return StanceList.IsValidIndex(Index) ? &StanceList[Index] : nullptr;
}
uint16 UCharacterDatabase::GetStanceIndex(FString StanceId) const {
  const uint16 *Index = StanceIdToIndex.Find(StanceId);
  return Index ? *Index : INDEX_NONE;
}
