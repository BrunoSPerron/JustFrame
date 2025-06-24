// Every Frame is a Choice //
#include "Data/Sources/IMoveDataSource.h"
#include "Data/LogCategories.h"
#include "Utilities/PayloadDeserializer.h"

bool IMoveDataSource::DeserializeMovePayloads(TArray<FMoveData> &OutMoves) {
  bool bSuccess = true;

  for (int32 i = 0; i < CanonicalMovePayloadArray.Num(); ++i) {
    const FString &PayloadString = CanonicalMovePayloadArray[i];
    if (PayloadString.IsEmpty()) {
      UE_LOG(MoveDBLog, Error, TEXT("Empty move payload string at index %d"), i);
      bSuccess = false;
      continue;
    }

    FDeserializedMovePayload Parsed;
    if (!FMovePayloadDeserializer::DeserializeMoveFromPayload(PayloadString, Parsed)) {
      UE_LOG(MoveDBLog, Error, TEXT("Failed to deserialize move payload at index %d"), i);
      bSuccess = false;
      continue;
    }

    MoveSourceVersions.Add(Parsed.Version);
    OutMoves.Append(Parsed.Moves);
  }

  return bSuccess;
}

bool IMoveDataSource::DeserializeStancePayloads(TArray<FStanceData> &OutStances) {
  bool bSuccess = true;

  for (int32 i = 0; i < CanonicalStancePayloadArray.Num(); ++i) {
    const FString &PayloadString = CanonicalStancePayloadArray[i];
    if (PayloadString.IsEmpty()) {
      UE_LOG(MoveDBLog, Error, TEXT("Empty stance payload string at index %d"), i);
      bSuccess = false;
      continue;
    }

    FDeserializedStancePayload Parsed;
    if (!FMovePayloadDeserializer::DeserializeStanceFromPayload(PayloadString, Parsed)) {
      UE_LOG(MoveDBLog, Error, TEXT("Failed to deserialize stance payload at index %d"), i);
      bSuccess = false;
      continue;
    }

    StanceSourceVersions.Add(Parsed.Version);
    OutStances.Append(Parsed.Stances);
  }

  return bSuccess;
}
