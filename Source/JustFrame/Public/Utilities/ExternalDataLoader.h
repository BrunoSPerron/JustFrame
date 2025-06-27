// Every Frame is a Choice //
#pragma once

#include "CoreMinimal.h"
#include "Data/Structs/ManifestStructs.h"
#include "Data/Structs/CharacterStructs.h"

struct ExternalDataLoader {
  static bool LoadManifest(FManifestData &OutManifest);

  static bool LoadCharacterData(const TArray<FString> &CharacterCollections,
                                TArray<FMoveData> &OutMoves, TArray<FStanceData> &OutStances);
};
