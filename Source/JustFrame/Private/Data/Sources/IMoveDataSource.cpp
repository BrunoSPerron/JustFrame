// Every Frame is a Choice //
#include "Data/Sources/IMoveDataSource.h"
#include "Data/LogCategories.h"
#include "Utilities/PayloadDeserializer.h"

bool IMoveDataSource::DeserializeCharacterPayloads(TArray<FMoveData> &OutMoves,
                                                   TArray<FStanceData> &OutStances) {
  bool bSuccess = true;

  for (int32 i = 0; i < CanonicalCharacterPayloadArray.Num(); ++i) {
    const FString &PayloadString = CanonicalCharacterPayloadArray[i];
    if (PayloadString.IsEmpty()) {
      UE_LOG(MoveDBLog, Error, TEXT("Empty character payload string at index %d"), i);
      bSuccess = false;
      continue;
    }

    FDeserializedCharacterPayload Parsed;
    if (!FMovePayloadDeserializer::DeserializeCharacterPayload(PayloadString, Parsed)) {
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
