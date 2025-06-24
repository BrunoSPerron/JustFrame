// Every Frame is a Choice //
#include "Utilities/PayloadDeserializer.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Dom/JsonObject.h"
#include "Dom/JsonValue.h"
#include "Data/LogCategories.h"
#include "JsonObjectConverter.h"

bool FMovePayloadDeserializer::DeserializeMoveFromPayload(const FString &CanonicalPayload,
                                                          FDeserializedMovePayload &OutData) {
  TSharedPtr<FJsonObject> Payload;
  TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(CanonicalPayload);
  if (!FJsonSerializer::Deserialize(Reader, Payload) || !Payload.IsValid()) {
    UE_LOG(MoveDBLog, Error, TEXT("Invalid JSON payload for move data"));
    return false;
  }

  if (!Payload->TryGetStringField(TEXT("version"), OutData.Version) || OutData.Version.IsEmpty()) {
    UE_LOG(MoveDBLog, Error, TEXT("Missing 'version' field in move payload"));
    return false;
  }

  const TArray<TSharedPtr<FJsonValue>> *RawMoveArray = nullptr;
  if (!Payload->TryGetArrayField(TEXT("moves"), RawMoveArray) || !RawMoveArray) {
    UE_LOG(MoveDBLog, Error, TEXT("Missing or invalid 'moves' array in move payload"));
    return false;
  }

  for (const TSharedPtr<FJsonValue> &Value : *RawMoveArray) {
    const TSharedPtr<FJsonObject> Object = Value->AsObject();
    if (!Object.IsValid()) {
      UE_LOG(MoveDBLog, Warning, TEXT("Invalid move entry (not an object)"));
      continue;
    }

    FMoveData ParsedMove;
    if (!FJsonObjectConverter::JsonObjectToUStruct(Object.ToSharedRef(), FMoveData::StaticStruct(),
                                                   &ParsedMove, 0, 0)) {
      UE_LOG(MoveDBLog, Warning, TEXT("Failed to parse move entry to struct"));
      continue;
    }

    OutData.Moves.Add(ParsedMove);
  }

  return true;
}

bool FMovePayloadDeserializer::DeserializeStanceFromPayload(const FString &CanonicalPayload,
                                                            FDeserializedStancePayload &OutData) {
  TSharedPtr<FJsonObject> Payload;
  TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(CanonicalPayload);
  if (!FJsonSerializer::Deserialize(Reader, Payload) || !Payload.IsValid()) {
    UE_LOG(MoveDBLog, Error, TEXT("Invalid JSON payload for stance data"));
    return false;
  }

  if (!Payload->TryGetStringField(TEXT("version"), OutData.Version) || OutData.Version.IsEmpty()) {
    UE_LOG(MoveDBLog, Error, TEXT("Missing 'version' field in stance payload"));
    return false;
  }

  const TArray<TSharedPtr<FJsonValue>> *RawStanceArray = nullptr;
  if (!Payload->TryGetArrayField(TEXT("stances"), RawStanceArray) || !RawStanceArray) {
    UE_LOG(MoveDBLog, Error, TEXT("Missing or invalid 'stances' array in stance payload"));
    return false;
  }

  for (const TSharedPtr<FJsonValue> &Value : *RawStanceArray) {
    const TSharedPtr<FJsonObject> Object = Value->AsObject();
    if (!Object.IsValid()) {
      UE_LOG(MoveDBLog, Warning, TEXT("Invalid stance entry (not an object)"));
      continue;
    }

    FStanceData ParsedStance;
    if (!FJsonObjectConverter::JsonObjectToUStruct(
            Object.ToSharedRef(), FStanceData::StaticStruct(), &ParsedStance, 0, 0)) {
      UE_LOG(MoveDBLog, Warning, TEXT("Failed to parse stance entry to struct"));
      continue;
    }

    OutData.Stances.Add(ParsedStance);
  }

  return true;
}