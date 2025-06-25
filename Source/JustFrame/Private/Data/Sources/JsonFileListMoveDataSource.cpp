// Every Frame is a Choice //
#include "Data/Sources/JsonFileListMoveDataSource.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Dom/JsonObject.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "JsonObjectConverter.h"
#include "Data/LogCategories.h"

// Every Frame is a Choice //
bool FJsonFileListMoveDataSource::LoadCharacterSources(const TArray<FString> &Collections,
                                                       TArray<FMoveData> &OutMoves,
                                                       TArray<FStanceData> &OutStances) {
  bool bSuccess = true;
  CharacterFilePaths.Reset();
  CharacterSourceVersions.Reset();

  TArray<FString> RawPayloads;

  for (const FString &Collection : Collections) {
    const FString FilePath =
        FPaths::ProjectDir() / TEXT("ExternalData/Core/Characters") / (Collection + TEXT(".json"));
    CharacterFilePaths.Add(FilePath);

    FString FileContents;
    if (!FFileHelper::LoadFileToString(FileContents, *FilePath)) {
      UE_LOG(MoveDBLog, Error,
             TEXT("FJsonFileListMoveDataSource::LoadCharacterSources Failed to load character "
                  "data file: %s"),
             *FilePath);
      RawPayloads.Add(TEXT(""));
      CharacterSourceVersions.Add(TEXT("invalid"));
      bSuccess = false;
      continue;
    }

    TSharedPtr<FJsonObject> Root;
    const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(FileContents);
    if (!FJsonSerializer::Deserialize(Reader, Root) || !Root.IsValid()) {
      UE_LOG(MoveDBLog, Error,
             TEXT("FJsonFileListMoveDataSource::LoadCharacterSources Invalid JSON in character "
                  "file: %s"),
             *FilePath);
      RawPayloads.Add(TEXT(""));
      CharacterSourceVersions.Add(TEXT("invalid"));
      bSuccess = false;
      continue;
    }

    FString Payload;
    {
      const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Payload);
      FJsonSerializer::Serialize(Root.ToSharedRef(), Writer);
    }

    RawPayloads.Add(Payload);

    FString Version;
    if (!Root->TryGetStringField(TEXT("version"), Version)) {
      Version = TEXT("invalid");
    }
    CharacterSourceVersions.Add(Version);
  }

  bSuccess &= DeserializeCharacterPayloads(RawPayloads, OutMoves, OutStances);
  return bSuccess;
}

int32 FJsonFileListMoveDataSource::GetCharacterSourceCount() const {
  return CharacterFilePaths.Num();
}

FString FJsonFileListMoveDataSource::GetCharacterSourceVersion(int32 FileIndex) const {
  return CharacterSourceVersions.IsValidIndex(FileIndex) ? CharacterSourceVersions[FileIndex]
                                                         : TEXT("invalid");
}
