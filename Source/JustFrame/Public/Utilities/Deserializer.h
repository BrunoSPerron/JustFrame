// Every Frame is a Choice //
#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonValue.h"
#include "Data/Structs/CharacterStructs.h"
#include "Data/Structs/ManifestStructs.h"

struct FDeserializer {
  static bool DeserializeCharacterData(const FString &Payload, FDeserializedCharacterData &OutData);

  static bool DeserializeCharacterList(const FString &Payload,
                                       TArray<FCharacterListEntry> &OutList);

  static bool DeserializeManifest(const FString &Payload, FManifestData &OutManifest);

private:
  static void ParseMoveArray(const TSharedPtr<FJsonObject> &Payload, TArray<FMoveData> &OutMoves);
  static void ParseStanceArray(const TSharedPtr<FJsonObject> &Payload,
                               TArray<FStanceData> &OutStances);
};
