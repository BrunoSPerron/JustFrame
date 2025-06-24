// Every Frame is a Choice //
#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonValue.h"
#include "Data/Structs/Structs_Move.h"

struct FDeserializedMovePayload {
  FString Version;
  TArray<FMoveData> Moves;
};

struct FDeserializedStancePayload {
  FString Version;
  TArray<FStanceData> Stances;
};

class FMovePayloadDeserializer {
public:
  static bool DeserializeMoveFromPayload(const FString &CanonicalPayload,
                                         FDeserializedMovePayload &OutData);
  static bool DeserializeStanceFromPayload(const FString &CanonicalPayload,
                                           FDeserializedStancePayload &OutData);
};
