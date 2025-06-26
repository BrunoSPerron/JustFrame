// Every Frame is a Choice //
#include "Utilities/Deserializer.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Dom/JsonObject.h"
#include "Dom/JsonValue.h"
#include "JsonObjectConverter.h"
#include "Data/LogCategories.h"

bool FMoveDeserializer::DeserializeCharacterPayload(const FString &CanonicalPayload,
                                                    FDeserializedCharacterPayload &OutData) {
  TSharedPtr<FJsonObject> Payload;
  TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(CanonicalPayload);
  if (!FJsonSerializer::Deserialize(Reader, Payload) || !Payload.IsValid()) {
    UE_LOG(MoveDBLog, Error, TEXT("Invalid JSON payload for character data"));
    return false;
  }

  if (!Payload->TryGetStringField(TEXT("version"), OutData.Version) || OutData.Version.IsEmpty()) {
    UE_LOG(MoveDBLog, Error, TEXT("Missing 'version' field in character payload"));
    return false;
  }

  ParseMoveArray(Payload, OutData.Moves);
  ParseStanceArray(Payload, OutData.Stances);

  return true;
}

void FMoveDeserializer::ParseMoveArray(const TSharedPtr<FJsonObject> &Payload,
                                       TArray<FMoveData> &OutMoves) {
  const TArray<TSharedPtr<FJsonValue>> *RawArray = nullptr;
  if (!Payload->TryGetArrayField(TEXT("moves"), RawArray) || !RawArray) {
    UE_LOG(MoveDBLog, Warning, TEXT("No 'moves' array found in character payload"));
    return;
  }

  for (const TSharedPtr<FJsonValue> &Value : *RawArray) {
    const TSharedPtr<FJsonObject> Object = Value->AsObject();
    if (!Object.IsValid()) {
      UE_LOG(MoveDBLog, Warning, TEXT("Invalid move entry (not an object)"));
      continue;
    }

    FMoveData Parsed;
    if (!FJsonObjectConverter::JsonObjectToUStruct(Object.ToSharedRef(), FMoveData::StaticStruct(),
                                                   &Parsed, 0, 0)) {
      UE_LOG(MoveDBLog, Warning, TEXT("Failed to parse move entry to struct"));
      continue;
    }

    OutMoves.Add(Parsed);
  }
}

void FMoveDeserializer::ParseStanceArray(const TSharedPtr<FJsonObject> &Payload,
                                         TArray<FStanceData> &OutStances) {
  const TArray<TSharedPtr<FJsonValue>> *RawArray = nullptr;
  if (!Payload->TryGetArrayField(TEXT("stances"), RawArray) || !RawArray) {
    UE_LOG(MoveDBLog, Warning, TEXT("No 'stances' array found in character payload"));
    return;
  }

  for (const TSharedPtr<FJsonValue> &Value : *RawArray) {
    const TSharedPtr<FJsonObject> Object = Value->AsObject();
    if (!Object.IsValid()) {
      UE_LOG(MoveDBLog, Warning, TEXT("Invalid stance entry (not an object)"));
      continue;
    }

    FStanceData Parsed;
    if (!FJsonObjectConverter::JsonObjectToUStruct(Object.ToSharedRef(),
                                                   FStanceData::StaticStruct(), &Parsed, 0, 0)) {
      UE_LOG(MoveDBLog, Warning, TEXT("Failed to parse stance entry to struct"));
      continue;
    }

    OutStances.Add(Parsed);
  }
}
