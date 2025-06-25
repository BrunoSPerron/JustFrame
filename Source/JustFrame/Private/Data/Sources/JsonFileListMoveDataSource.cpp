// Every Frame is a Choice //
#include "Data/Sources/JsonFileListMoveDataSource.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Dom/JsonObject.h"
#include "Dom/JsonValue.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "JsonObjectConverter.h"
#include "Data/LogCategories.h"

bool FJsonFileListMoveDataSource::LoadCharacterSources(const TArray<FString> &Collections) {
  bool bSuccess = true;

  CharacterFilePaths.Reset();
  for (const FString &Collection : Collections) {
    CharacterFilePaths.Add(FPaths::ProjectDir() / TEXT("ExternalData/Characters") /
                           (Collection + TEXT(".json")));
  }

  CanonicalCharacterPayloadArray.Reset();
  ClaimedCharacterSignatureArray.Reset();
  CharacterSourceVersions.Reset();

  for (const FString &FilePath : CharacterFilePaths) {
    FString FileContents;
    if (!FFileHelper::LoadFileToString(FileContents, *FilePath)) {
      UE_LOG(MoveDBLog, Error, TEXT("Failed to load character data file: %s"), *FilePath);
      CanonicalCharacterPayloadArray.Add(TEXT(""));
      ClaimedCharacterSignatureArray.Add(TEXT(""));
      CharacterSourceVersions.Add(TEXT("invalid"));
      bSuccess = false;
      continue;
    }

    TSharedPtr<FJsonObject> Root;
    const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(FileContents);
    if (!FJsonSerializer::Deserialize(Reader, Root) || !Root.IsValid()) {
      UE_LOG(MoveDBLog, Error, TEXT("Invalid JSON in character file: %s"), *FilePath);
      CanonicalCharacterPayloadArray.Add(TEXT(""));
      ClaimedCharacterSignatureArray.Add(TEXT(""));
      CharacterSourceVersions.Add(TEXT("invalid"));
      bSuccess = false;
      continue;
    }

    const TSharedPtr<FJsonObject> Payload = Root->GetObjectField(TEXT("payload"));
    if (!Payload.IsValid()) {
      UE_LOG(MoveDBLog, Error, TEXT("Missing 'payload' in character file: %s"), *FilePath);
      CanonicalCharacterPayloadArray.Add(TEXT(""));
      ClaimedCharacterSignatureArray.Add(TEXT(""));
      CharacterSourceVersions.Add(TEXT("invalid"));
      bSuccess = false;
      continue;
    }

    FString CanonicalPayload;
    {
      const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&CanonicalPayload);
      FJsonSerializer::Serialize(Payload.ToSharedRef(), Writer);
    }

    CanonicalCharacterPayloadArray.Add(CanonicalPayload);

    FString Signature;
    if (!Root->TryGetStringField(TEXT("signature"), Signature)) {
      Signature = TEXT("");
    }
    ClaimedCharacterSignatureArray.Add(Signature);

    FString Version;
    if (!Payload->TryGetStringField(TEXT("version"), Version)) {
      Version = TEXT("invalid");
    }
    CharacterSourceVersions.Add(Version);
  }

  return bSuccess;
}

FString FJsonFileListMoveDataSource::GetCharacterSourceName() const {
  return FString::Printf(TEXT("JSONFileList:Characters[%d files]"),
                         CanonicalCharacterPayloadArray.Num());
}

int32 FJsonFileListMoveDataSource::GetCharacterSourceCount() const {
  return CanonicalCharacterPayloadArray.Num();
}

FString FJsonFileListMoveDataSource::GetCharacterRawPayload(int32 FileIndex) const {
  return CanonicalCharacterPayloadArray.IsValidIndex(FileIndex)
             ? CanonicalCharacterPayloadArray[FileIndex]
             : TEXT("");
}

FString FJsonFileListMoveDataSource::GetCharacterClaimedSignature(int32 FileIndex) const {
  return ClaimedCharacterSignatureArray.IsValidIndex(FileIndex)
             ? ClaimedCharacterSignatureArray[FileIndex]
             : TEXT("");
}

FString FJsonFileListMoveDataSource::GetCharacterSourceVersion(int32 FileIndex) const {
  return CharacterSourceVersions.IsValidIndex(FileIndex) ? CharacterSourceVersions[FileIndex]
                                                         : TEXT("invalid");
}
