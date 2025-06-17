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
    FString Path = FPaths::ProjectDir() / TEXT("ExternalData/Moves") / (Collection + TEXT(".json"));
    FilePaths.Add(Path);
  }

  RawMovesJsonArray.Reset();
  ClaimedSignatureArray.Reset();

  RawMovesJsonArray.Reserve(FilePaths.Num());
  ClaimedSignatureArray.Reserve(FilePaths.Num());

  for (const FString &FilePath : FilePaths) {
    FString FileContents;
    if (!FFileHelper::LoadFileToString(FileContents, *FilePath)) {
      UE_LOG(MoveDBLog, Error, TEXT("Failed to load move data file: %s"), *FilePath);
      bSuccess = false;
      RawMovesJsonArray.Add(FString());
      ClaimedSignatureArray.Add(FString());
      continue;
    }

    TSharedPtr<FJsonObject> Root;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(FileContents);
    if (!FJsonSerializer::Deserialize(Reader, Root) || !Root.IsValid()) {
      UE_LOG(MoveDBLog, Error, TEXT("Invalid JSON in: %s"), *FilePath);
      bSuccess = false;
      RawMovesJsonArray.Add(FString());
      ClaimedSignatureArray.Add(FString());
      continue;
    }

    const TArray<TSharedPtr<FJsonValue>> *MovesArray = nullptr;
    if (!Root->TryGetArrayField(TEXT("moves"), MovesArray)) {
      UE_LOG(MoveDBLog, Error, TEXT("Missing 'moves' array in: %s"), *FilePath);
      bSuccess = false;
      RawMovesJsonArray.Add(FString());
      ClaimedSignatureArray.Add(FString());
      continue;
    }

    FString CanonicalMovesString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&CanonicalMovesString);
    FJsonSerializer::Serialize(MakeShared<FJsonValueArray>(*MovesArray), TEXT(""), Writer);
    Writer->Close();

    RawMovesJsonArray.Add(CanonicalMovesString);

    FString SignatureString;
    if (!Root->TryGetStringField(TEXT("signature"), SignatureString)) {
      UE_LOG(MoveDBLog, Warning, TEXT("Missing 'signature' field in: %s"), *FilePath);
      SignatureString = FString();
    }
    ClaimedSignatureArray.Add(SignatureString);
  }

  return bSuccess;
}

bool FJsonFileListMoveDataSource::ParseMoves(TArray<FMoveData> &OutMoves) {
  bool bSuccess = true;

  for (int32 i = 0; i < RawMovesJsonArray.Num(); ++i) {
    const FString &JsonArrayString = RawMovesJsonArray[i];
    if (JsonArrayString.IsEmpty()) continue;

    TArray<TSharedPtr<FJsonValue>> MovesArray;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonArrayString);
    if (!FJsonSerializer::Deserialize(Reader, MovesArray)) {
      UE_LOG(MoveDBLog, Warning, TEXT("Failed to parse move array from file index %d"), i);
      bSuccess = false;
      continue;
    }

    for (const TSharedPtr<FJsonValue> &MoveValue : MovesArray) {
      TSharedPtr<FJsonObject> MoveObject = MoveValue->AsObject();
      if (!MoveObject.IsValid()) {
        UE_LOG(MoveDBLog, Warning, TEXT("Invalid move object at file index %d"), i);
        bSuccess = false;
        continue;
      }

      FMoveData Move;
      if (!FJsonObjectConverter::JsonObjectToUStruct(MoveObject.ToSharedRef(),
                                                     FMoveData::StaticStruct(), &Move, 0, 0)) {
        UE_LOG(MoveDBLog, Warning, TEXT("Failed to convert move struct at file index %d"), i);
        bSuccess = false;
        continue;
      }

      OutMoves.Add(Move);
    }
  }

  return bSuccess;
}

FString FJsonFileListMoveDataSource::GetRawMovesSection(int32 FileIndex) const {
  if (RawMovesJsonArray.IsValidIndex(FileIndex)) {
    return RawMovesJsonArray[FileIndex];
  }
  return FString();
}

FString FJsonFileListMoveDataSource::GetClaimedSignature(int32 FileIndex) const {
  if (ClaimedSignatureArray.IsValidIndex(FileIndex)) {
    return ClaimedSignatureArray[FileIndex];
  }
  return FString();
}
