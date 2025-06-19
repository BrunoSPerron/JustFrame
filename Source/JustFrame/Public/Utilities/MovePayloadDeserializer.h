// Every Frame is a Choice //
#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonValue.h"

struct FDeserializedMovePayload {
  FString Version;
  TArray<TSharedPtr<FJsonValue>> MovesArray;
};

class FMovePayloadDeserializer {
public:
  static bool DeserializeFromPayload(const FString &CanonicalPayload,
                                     FDeserializedMovePayload &OutData);
};
