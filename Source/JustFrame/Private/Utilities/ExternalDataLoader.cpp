// Every Frame is a Choice //
#include "Utilities/ExternalDataLoader.h"
#include "Data/LogCategories.h"
#include "Utilities/Deserializer.h"

#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Dom/JsonObject.h"

#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Modules/ModuleManager.h"
#include "Engine/Texture2D.h"

UTexture2D *ExternalDataLoader::LoadTexture(const FString &RelativePath) {
  const FString AbsolutePath = FPaths::ProjectDir() / RelativePath;

  TArray<uint8> RawFileData;
  if (!FFileHelper::LoadFileToArray(RawFileData, *AbsolutePath)) return nullptr;

  IImageWrapperModule &ImageWrapperModule =
      FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
  TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);

  if (!ImageWrapper.IsValid() ||
      !ImageWrapper->SetCompressed(RawFileData.GetData(), RawFileData.Num()))
    return nullptr;

  TArray64<uint8> UncompressedBGRA;
  if (!ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, UncompressedBGRA)) return nullptr;

  UTexture2D *Texture =
      UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_B8G8R8A8);
  if (!Texture) return nullptr;

  void *TextureData = Texture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
  FMemory::Memcpy(TextureData, UncompressedBGRA.GetData(), UncompressedBGRA.Num());

  Texture->GetPlatformData()->Mips[0].BulkData.Unlock();

  Texture->UpdateResource();
  return Texture;
}

bool ExternalDataLoader::LoadCharacterList(TArray<FCharacterListEntry> &OutCharacterList) {
  const FString CharacterListPath =
      FPaths::ProjectDir() / TEXT("ExternalData/Core/Characters/_CharacterList.json");

  FString FileContents;
  if (!FFileHelper::LoadFileToString(FileContents, *CharacterListPath)) {
    UE_LOG(DataLoaderLog, Error, TEXT("ExternalDataLoader::LoadCharacterList Failed to read: %s"),
           *CharacterListPath);
    return false;
  }

  if (!FDeserializer::DeserializeCharacterList(FileContents, OutCharacterList)) {
    UE_LOG(DataLoaderLog, Error,
           TEXT("ExternalDataLoader::LoadCharacterList Failed to deserialize: %s"),
           *CharacterListPath);
    return false;
  }

  return true;
}

bool ExternalDataLoader::LoadCharacterCards(const TArray<FCharacterListEntry> &CharacterList,
                                            TMap<FString, UTexture2D *> &OutCards) {
  bool bSuccess = true;

  for (const auto &Entry : CharacterList) {
    const FString CardPath = TEXT("ExternalData/Core/") + Entry.Card;

    UTexture2D *Texture = LoadTexture(CardPath);
    if (!Texture) {
      UE_LOG(DataLoaderLog, Error,
             TEXT("ExternalDataLoader::LoadCharacterCards Failed to load texture: %s"), *CardPath);
      bSuccess = false;
      continue;
    }

    OutCards.Add(Entry.Id, Texture);
  }

  return bSuccess;
}

bool ExternalDataLoader::LoadManifest(FManifestData &OutManifest) {
  const FString ManifestPath = FPaths::ProjectDir() / TEXT("ExternalData/_Manifest.json");

  FString FileContents;
  if (!FFileHelper::LoadFileToString(FileContents, *ManifestPath)) {
    UE_LOG(DataLoaderLog, Error,
           TEXT("ExternalDataLoader::LoadManifest Failed to read manifest: %s"), *ManifestPath);
    return false;
  }

  if (!FDeserializer::DeserializeManifest(FileContents, OutManifest)) {
    UE_LOG(DataLoaderLog, Error,
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

  for (const FString &CollectionName : CharacterCollections) {
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
