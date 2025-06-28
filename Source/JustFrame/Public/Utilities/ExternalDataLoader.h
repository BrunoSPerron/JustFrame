// Every Frame is a Choice //
#pragma once

#include "CoreMinimal.h"
#include "Data/Structs/ManifestStructs.h"
#include "Data/Structs/CharacterStructs.h"

struct ExternalDataLoader {
  static bool LoadManifest(FManifestData& OutManifest);

  static bool LoadCharacterList(TArray<FCharacterListEntry>& OutCharacterList);

  static bool LoadCharacterData(const TArray<FString>& CharacterCollections,
                                TArray<FMoveData>& OutMoves,
                                TArray<FStanceData>& OutStances);

  static bool LoadCharacterCards(const TArray<FCharacterListEntry>& CharacterList,
                                 TMap<FString, UTexture2D*>& OutCards);

  static UTexture2D* LoadTexture(const FString& RelativePath);
};
