// Every Frame is a Choice //
#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonValue.h"
#include "Data/Structs/Structs_Character.h"

struct FDeserializedCharacterPayload {
  FString Version;
  TArray<FMoveData> Moves;
  TArray<FStanceData> Stances;
};

struct FMovePayloadDeserializer {
  static bool DeserializeCharacterPayload(const FString &CanonicalPayload,
                                          FDeserializedCharacterPayload &OutData);

private:
  static void ParseMoveArray(const TSharedPtr<FJsonObject> &Payload, TArray<FMoveData> &OutMoves);
  static void ParseStanceArray(const TSharedPtr<FJsonObject> &Payload,
                               TArray<FStanceData> &OutStances);
};
