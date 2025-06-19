// Every Frame is a Choice //
#include "Characters/State/JsonFileListMoveDataSource.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Dom/JsonObject.h"
#include "Dom/JsonValue.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "JsonObjectConverter.h"
#include "Data/LogCategories.h"

FString FJsonFileListMoveDataSource::GetSourceName() const {
  return FString::Printf(TEXT("JSONFileList[%d files]"), FilePaths.Num());
}

bool FJsonFileListMoveDataSource::LoadSources(const TArray<FString> &Collections) {
  bool bSuccess = true;

  FilePaths.Reset();
  for (const FString &Collection : Collections) {
    FilePaths.Add(FPaths::ProjectDir() / TEXT("ExternalData/Moves") / (Collection + TEXT(".json")));
  }

  CanonicalPayloadArray.Reset();
  ClaimedSignatureArray.Reset();
  SourceVersions.Reset();

  CanonicalPayloadArray.Reserve(FilePaths.Num());
  ClaimedSignatureArray.Reserve(FilePaths.Num());
  SourceVersions.Reserve(FilePaths.Num());

  for (const FString &FilePath : FilePaths) {
    FString FileContents;
    if (!FFileHelper::LoadFileToString(FileContents, *FilePath)) {
      UE_LOG(MoveDBLog, Error, TEXT("Failed to load move data file: %s"), *FilePath);
      bSuccess = false;
      CanonicalPayloadArray.Add(TEXT(""));
      ClaimedSignatureArray.Add(TEXT(""));
      SourceVersions.Add(TEXT("invalid"));
      continue;
    }

    TSharedPtr<FJsonObject> Root;
    const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(FileContents);
    if (!FJsonSerializer::Deserialize(Reader, Root) || !Root.IsValid()) {
      UE_LOG(MoveDBLog, Error, TEXT("Invalid JSON in: %s"), *FilePath);
      bSuccess = false;
      CanonicalPayloadArray.Add(TEXT(""));
      ClaimedSignatureArray.Add(TEXT(""));
      SourceVersions.Add(TEXT("invalid"));
      continue;
    }

    const TSharedPtr<FJsonObject> Payload = Root->GetObjectField(TEXT("payload"));
    if (!Payload.IsValid()) {
      UE_LOG(MoveDBLog, Error, TEXT("Missing 'payload' object in: %s"), *FilePath);
      bSuccess = false;
      CanonicalPayloadArray.Add(TEXT(""));
      ClaimedSignatureArray.Add(TEXT(""));
      SourceVersions.Add(TEXT("invalid"));
      continue;
    }

    FString CanonicalPayload;
    {
      const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&CanonicalPayload);
      FJsonSerializer::Serialize(Payload.ToSharedRef(), Writer);
    }
    CanonicalPayloadArray.Add(CanonicalPayload);

    FString SignatureString;
    if (!Root->TryGetStringField(TEXT("signature"), SignatureString)) {
      UE_LOG(MoveDBLog, Warning, TEXT("Missing 'signature' field in: %s"), *FilePath);
      SignatureString = TEXT("");
    }
    ClaimedSignatureArray.Add(SignatureString);
  }

  return bSuccess;
}

FString FJsonFileListMoveDataSource::GetRawPayload(int32 FileIndex) const {
  if (CanonicalPayloadArray.IsValidIndex(FileIndex)) {
    return CanonicalPayloadArray[FileIndex];
  }
  return FString();
}

FString FJsonFileListMoveDataSource::GetClaimedSignature(int32 FileIndex) const {
  if (ClaimedSignatureArray.IsValidIndex(FileIndex)) {
    return ClaimedSignatureArray[FileIndex];
  }
  return FString();
}

FString FJsonFileListMoveDataSource::GetSourceVersion(int32 FileIndex) const {
  if (SourceVersions.IsValidIndex(FileIndex)) {
    return SourceVersions[FileIndex];
  }
  return TEXT("invalid");
}
