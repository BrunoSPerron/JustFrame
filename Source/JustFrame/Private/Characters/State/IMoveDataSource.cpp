#include "Characters/State/IMoveDataSource.h"
#include "Data/LogCategories.h"
#include "Dom/JsonObject.h"
#include "Dom/JsonValue.h"
#include "Serialization/JsonReader.h"
#include "JsonObjectConverter.h"

bool IMoveDataSource::DeserializePayloads(TArray<FMoveData> &OutMoves) {
  bool bSuccess = true;

  for (int32 i = 0; i < CanonicalPayloadArray.Num(); ++i) {
    const FString &PayloadString = CanonicalPayloadArray[i];
    if (PayloadString.IsEmpty()) {
      bSuccess = false;
      continue;
    }

    TSharedPtr<FJsonObject> Payload;
    const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(PayloadString);
    if (!FJsonSerializer::Deserialize(Reader, Payload) || !Payload.IsValid()) {
      UE_LOG(MoveDBLog, Error, TEXT("Failed to parse payload JSON at file index %d"), i);
      bSuccess = false;
      continue;
    }

    FString VersionString;
    if (Payload->TryGetStringField(TEXT("version"), VersionString)) {
      SourceVersions.Add(VersionString);
    } else {
      UE_LOG(MoveDBLog, Error, TEXT("Missing 'version' in payload at file index %d"), i);
      bSuccess = false;
      continue;
    }

    const TArray<TSharedPtr<FJsonValue>> *MovesArray = nullptr;
    if (!Payload->TryGetArrayField(TEXT("moves"), MovesArray)) {
      UE_LOG(MoveDBLog, Error, TEXT("Missing 'moves' array in payload at file index %d"), i);
      bSuccess = false;
      continue;
    }

    for (const TSharedPtr<FJsonValue> &MoveValue : *MovesArray) {
      TSharedPtr<FJsonObject> MoveObject = MoveValue->AsObject();
      if (!MoveObject.IsValid()) {
        UE_LOG(MoveDBLog, Warning, TEXT("Invalid move object at file index %d"), i);
        bSuccess = false;
        continue;
      }

      FMoveData Move;
      if (!FJsonObjectConverter::JsonObjectToUStruct(MoveObject.ToSharedRef(),
                                                     FMoveData::StaticStruct(), &Move)) {
        UE_LOG(MoveDBLog, Warning, TEXT("Failed to convert move struct at file index %d"), i);
        bSuccess = false;
        continue;
      }

      OutMoves.Add(Move);
    }
  }

  return bSuccess;
}