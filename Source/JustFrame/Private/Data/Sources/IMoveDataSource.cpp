// Every Frame is a Choice //
#include "Data/Sources/IMoveDataSource.h"
#include "Data/LogCategories.h"
#include "Utilities/PayloadDeserializer.h"

bool IMoveDataSource::DeserializeCharacterPayloads(const TArray<FString> &RawJsonPayloads,
                                                   TArray<FMoveData> &OutMoves,
                                                   TArray<FStanceData> &OutStances) {
  bool bSuccess = true;

  for (int32 i = 0; i < RawJsonPayloads.Num(); ++i) {
    const FString &JsonString = RawJsonPayloads[i];
    if (JsonString.IsEmpty()) {
      UE_LOG(MoveDBLog, Error, TEXT("Empty character JSON string at index %d"), i);
      bSuccess = false;
      continue;
    }

    FDeserializedCharacterPayload Parsed;
    if (!FMovePayloadDeserializer::DeserializeCharacterPayload(JsonString, Parsed)) {
      UE_LOG(MoveDBLog, Error, TEXT("Failed to deserialize character payload at index %d"), i);
      bSuccess = false;
      continue;
    }

    CharacterSourceVersions.Add(Parsed.Version);
    OutMoves.Append(Parsed.Moves);
    OutStances.Append(Parsed.Stances);
  }

  return bSuccess;
}
