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

FString FJsonFileListMoveDataSource::GetMoveSourceName() const {
  return FString::Printf(TEXT("JSONFileList:Moves[%d files]"), CanonicalMovePayloadArray.Num());
}

FString FJsonFileListMoveDataSource::GetStanceSourceName() const {
  return FString::Printf(TEXT("JSONFileList:Stances[%d files]"), CanonicalStancePayloadArray.Num());
}

bool FJsonFileListMoveDataSource::LoadMoveSources(const TArray<FString> &Collections) {
  bool bSuccess = true;

  MoveFilePaths.Reset();
  for (const FString &Collection : Collections) {
    MoveFilePaths.Add(FPaths::ProjectDir() / TEXT("ExternalData/Moves") /
                      (Collection + TEXT(".json")));
  }

  CanonicalMovePayloadArray.Reset();
  ClaimedMoveSignatureArray.Reset();
  MoveSourceVersions.Reset();

  for (const FString &FilePath : MoveFilePaths) {
    FString FileContents;
    if (!FFileHelper::LoadFileToString(FileContents, *FilePath)) {
      UE_LOG(MoveDBLog, Error, TEXT("Failed to load move data file: %s"), *FilePath);
      bSuccess = false;
      continue;
    }

    TSharedPtr<FJsonObject> Root;
    const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(FileContents);
    if (!FJsonSerializer::Deserialize(Reader, Root) || !Root.IsValid()) {
      UE_LOG(MoveDBLog, Error, TEXT("Invalid JSON in: %s"), *FilePath);
      bSuccess = false;
      continue;
    }

    const TSharedPtr<FJsonObject> Payload = Root->GetObjectField(TEXT("payload"));
    if (!Payload.IsValid() || !Payload->HasField(TEXT("moves"))) {
      UE_LOG(MoveDBLog, Warning, TEXT("File does not contain move payload: %s"), *FilePath);
      continue;
    }

    FString CanonicalPayload;
    {
      const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&CanonicalPayload);
      FJsonSerializer::Serialize(Payload.ToSharedRef(), Writer);
    }

    CanonicalMovePayloadArray.Add(CanonicalPayload);

    FString SignatureString;
    Root->TryGetStringField(TEXT("signature"), SignatureString);
    ClaimedMoveSignatureArray.Add(SignatureString);
  }
  return bSuccess;
}

bool FJsonFileListMoveDataSource::LoadStanceSources(const TArray<FString> &Collections) {
  bool bSuccess = true;

  StanceFilePaths.Reset();
  for (const FString &Collection : Collections) {
    StanceFilePaths.Add(FPaths::ProjectDir() / TEXT("ExternalData/Stances") /
                        (Collection + TEXT(".json")));
  }

  CanonicalStancePayloadArray.Reset();
  ClaimedStanceSignatureArray.Reset();
  StanceSourceVersions.Reset();

  CanonicalStancePayloadArray.Reserve(StanceFilePaths.Num());
  ClaimedStanceSignatureArray.Reserve(StanceFilePaths.Num());
  StanceSourceVersions.Reserve(StanceFilePaths.Num());

  for (const FString &FilePath : StanceFilePaths) {
    FString FileContents;
    if (!FFileHelper::LoadFileToString(FileContents, *FilePath)) {
      UE_LOG(MoveDBLog, Error, TEXT("Failed to load stance data file: %s"), *FilePath);
      CanonicalStancePayloadArray.Add(TEXT(""));
      ClaimedStanceSignatureArray.Add(TEXT(""));
      StanceSourceVersions.Add(TEXT("invalid"));
      bSuccess = false;
      continue;
    }

    TSharedPtr<FJsonObject> Root;
    const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(FileContents);
    if (!FJsonSerializer::Deserialize(Reader, Root) || !Root.IsValid()) {
      UE_LOG(MoveDBLog, Error, TEXT("Invalid JSON in stance file: %s"), *FilePath);
      CanonicalStancePayloadArray.Add(TEXT(""));
      ClaimedStanceSignatureArray.Add(TEXT(""));
      StanceSourceVersions.Add(TEXT("invalid"));
      bSuccess = false;
      continue;
    }

    const TSharedPtr<FJsonObject> Payload = Root->GetObjectField(TEXT("payload"));
    if (!Payload.IsValid()) {
      UE_LOG(MoveDBLog, Error, TEXT("Missing 'payload' in stance file: %s"), *FilePath);
      CanonicalStancePayloadArray.Add(TEXT(""));
      ClaimedStanceSignatureArray.Add(TEXT(""));
      StanceSourceVersions.Add(TEXT("invalid"));
      bSuccess = false;
      continue;
    }

    FString CanonicalPayload;
    const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&CanonicalPayload);
    FJsonSerializer::Serialize(Payload.ToSharedRef(), Writer);
    CanonicalStancePayloadArray.Add(CanonicalPayload);

    FString Signature;
    if (!Root->TryGetStringField(TEXT("signature"), Signature)) {
      Signature = TEXT("");
    }
    ClaimedStanceSignatureArray.Add(Signature);
  }
  return bSuccess;
}

// Move accessors
FString FJsonFileListMoveDataSource::GetMoveRawPayload(int32 FileIndex) const {
  return CanonicalMovePayloadArray.IsValidIndex(FileIndex) ? CanonicalMovePayloadArray[FileIndex]
                                                           : TEXT("");
}
FString FJsonFileListMoveDataSource::GetMoveClaimedSignature(int32 FileIndex) const {
  return ClaimedMoveSignatureArray.IsValidIndex(FileIndex) ? ClaimedMoveSignatureArray[FileIndex]
                                                           : TEXT("");
}
FString FJsonFileListMoveDataSource::GetMoveSourceVersion(int32 FileIndex) const {
  return MoveSourceVersions.IsValidIndex(FileIndex) ? MoveSourceVersions[FileIndex]
                                                    : TEXT("invalid");
}
int32 FJsonFileListMoveDataSource::GetMoveSourceCount() const {
  return CanonicalMovePayloadArray.Num();
}

// Stance accessors
FString FJsonFileListMoveDataSource::GetStanceRawPayload(int32 FileIndex) const {
  return CanonicalStancePayloadArray.IsValidIndex(FileIndex)
             ? CanonicalStancePayloadArray[FileIndex]
             : TEXT("");
}
FString FJsonFileListMoveDataSource::GetStanceClaimedSignature(int32 FileIndex) const {
  return ClaimedStanceSignatureArray.IsValidIndex(FileIndex)
             ? ClaimedStanceSignatureArray[FileIndex]
             : TEXT("");
}
FString FJsonFileListMoveDataSource::GetStanceSourceVersion(int32 FileIndex) const {
  return StanceSourceVersions.IsValidIndex(FileIndex) ? StanceSourceVersions[FileIndex]
                                                      : TEXT("invalid");
}
int32 FJsonFileListMoveDataSource::GetStanceSourceCount() const {
  return CanonicalStancePayloadArray.Num();
}
