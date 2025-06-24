// Every Frame is a Choice //
#include "Data/CharacterDataDatabase.h"
#include "Data/Sources/JsonFileListMoveDataSource.h"
#include "Utilities/CryptoUtils.h"
#include "Data/LogCategories.h"
#include "Data/Consts.h"

void UCharacterDataDatabase::LoadCharacterData(const TArray<FString> &MoveCollections,
                                               const TArray<FString> &StanceCollections) {
  DataSource = MakeUnique<FJsonFileListMoveDataSource>();

  // LOAD MOVE SOURCES
  if (!DataSource->LoadMoveSources(MoveCollections)) {
    UE_LOG(MoveDBLog, Error, TEXT("Failed to load move sources."));
  }

  for (int32 i = 0; i < DataSource->GetMoveSourceCount(); ++i) {
    const FString Raw = DataSource->GetMoveRawPayload(i);
    const FString Sig = DataSource->GetMoveClaimedSignature(i);
    if (!FCryptoUtils::VerifySignatureECDSA(MOVE_DATA_PUBLIC_KEY_PEM, Raw, Sig)) {
      UE_LOG(MoveDBLog, Warning, TEXT("Move signature failed at index %d"), i);
      // TODO:
      // -Add a label in local play
      // -Download valid values when online
    }
  }

  TArray<FMoveData> LoadedMoves;
  if (DataSource->DeserializeMovePayloads(LoadedMoves)) {
    MoveMap.Empty();
    MoveList.Empty();
    MoveIdToIndex.Empty();

    for (int32 i = 0; i < LoadedMoves.Num(); ++i) {
      const FMoveData &Move = LoadedMoves[i];
      MoveMap.Add(Move.MoveID, Move);
      MoveIdToIndex.Add(Move.MoveID, i);
      MoveList.Add(Move);
    }
  }

  // LOAD STANCE SOURCES
  if (!DataSource->LoadStanceSources(StanceCollections)) {
    UE_LOG(MoveDBLog, Error, TEXT("Failed to load stance sources."));
  }

  for (int32 i = 0; i < DataSource->GetStanceSourceCount(); ++i) {
    const FString Raw = DataSource->GetStanceRawPayload(i);
    const FString Sig = DataSource->GetStanceClaimedSignature(i);
    if (!FCryptoUtils::VerifySignatureECDSA(MOVE_DATA_PUBLIC_KEY_PEM, Raw, Sig)) {
      UE_LOG(MoveDBLog, Warning, TEXT("Stance signature failed at index %d"), i);
      // TODO:
      // -Add a label in local play
      // -Download valid values when online
    }
  }

  TArray<FStanceData> LoadedStances;
  if (DataSource->DeserializeStancePayloads(LoadedStances)) {
    StanceMap.Empty();
    StanceList.Empty();
    StanceIdToIndex.Empty();

    for (int32 i = 0; i < LoadedStances.Num(); ++i) {
      const FStanceData &Stance = LoadedStances[i];
      StanceMap.Add(Stance.StanceId, Stance);
      StanceIdToIndex.Add(Stance.StanceId, i);
      StanceList.Add(Stance);
    }
  }

  UE_LOG(MoveDBLog, Log, TEXT("Character data loaded: %d moves, %d stances"), MoveList.Num(),
         StanceList.Num());
}

// Move
const FMoveData *UCharacterDataDatabase::FindMove(FName MoveID) const {
  return MoveMap.Find(MoveID);
}

const FMoveData *UCharacterDataDatabase::FindMoveByIndex(uint16 Index) const {
  return MoveList.IsValidIndex(Index) ? &MoveList[Index] : nullptr;
}

uint16 UCharacterDataDatabase::GetMoveIndex(FName MoveID) const {
  const uint16 *Index = MoveIdToIndex.Find(MoveID);
  return Index ? *Index : INDEX_NONE;
}

// Stance
const FStanceData *UCharacterDataDatabase::FindStance(FString StanceId) const {
  return StanceMap.Find(StanceId);
}

const FStanceData *UCharacterDataDatabase::FindStanceByIndex(uint16 Index) const {
  return StanceList.IsValidIndex(Index) ? &StanceList[Index] : nullptr;
}

uint16 UCharacterDataDatabase::GetStanceIndex(FString StanceId) const {
  const uint16 *Index = StanceIdToIndex.Find(StanceId);
  return Index ? *Index : INDEX_NONE;
}
