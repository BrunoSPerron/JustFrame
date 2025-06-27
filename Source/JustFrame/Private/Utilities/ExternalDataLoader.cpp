// Every Frame is a Choice //
#include "Utilities/ExternalDataLoader.h"
#include "Utilities/Deserializer.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Dom/JsonObject.h"
#include "Data/LogCategories.h"

bool ExternalDataLoader::LoadManifest(FManifestData &OutManifest) {
  const FString ManifestPath = FPaths::ProjectDir() / TEXT("ExternalData/_Manifest.json");

  FString FileContents;
  if (!FFileHelper::LoadFileToString(FileContents, *ManifestPath)) {
    UE_LOG(LogTemp, Error, TEXT("ExternalDataLoader::LoadManifest Failed to read manifest: %s"),
           *ManifestPath);
    return false;
  }

  if (!FDeserializer::DeserializeManifest(FileContents, OutManifest)) {
    UE_LOG(LogTemp, Error,
           TEXT("ExternalDataLoader::LoadManifest Failed to deserialize manifest: %s"),
           *ManifestPath);
    return false;
  }

  return true;
}

bool ExternalDataLoader::LoadCharacterData(const TArray<FString> &CharacterCollections,
                                           TArray<FMoveData> &OutMoves,
                                           TArray<FStanceData> &OutStances) {
  bool bSuccess = true;

  for (int32 i = 0; i < CharacterCollections.Num(); ++i) {
    const FString &CollectionName = CharacterCollections[i];
    const FString FilePath = FPaths::ProjectDir() / TEXT("ExternalData/Core/Characters") /
                             (CollectionName + TEXT(".json"));

    FString FileContents;
    if (!FFileHelper::LoadFileToString(FileContents, *FilePath)) {
      UE_LOG(CharacterDBLog, Error,
             TEXT("ExternalDataLoader::LoadCharacterData Failed to read file: %s"), *FilePath);
      bSuccess = false;
      continue;
    }

    FDeserializedCharacterData Parsed;
    if (!FDeserializer::DeserializeCharacterData(FileContents, Parsed)) {
      UE_LOG(CharacterDBLog, Error,
             TEXT("ExternalDataLoader::LoadCharacterData Failed to deserialize: %s"), *FilePath);
      bSuccess = false;
      continue;
    }

    OutMoves.Append(Parsed.Moves);
    OutStances.Append(Parsed.Stances);
  }

  return bSuccess;
}
