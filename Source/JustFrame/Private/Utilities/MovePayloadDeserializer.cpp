// Every Frame is a Choice //
#include "Utilities/MovePayloadDeserializer.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Dom/JsonObject.h"
#include "Dom/JsonValue.h"
#include "Data/LogCategories.h"
#include "JsonObjectConverter.h"

bool FMovePayloadDeserializer::DeserializeFromPayload(const FString &CanonicalPayload,
                                                      FDeserializedMovePayload &OutData) {
  TSharedPtr<FJsonObject> Payload;
  TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(CanonicalPayload);
  if (!FJsonSerializer::Deserialize(Reader, Payload) || !Payload.IsValid()) {
    UE_LOG(MoveDBLog, Error, TEXT("Invalid JSON payload"));
    return false;
  }

  if (!Payload->TryGetStringField(TEXT("version"), OutData.Version) || OutData.Version.IsEmpty()) {
    UE_LOG(MoveDBLog, Error, TEXT("Missing 'version' field in payload"));
    return false;
  }

  const TArray<TSharedPtr<FJsonValue>> *RawMoveArray = nullptr;
  if (!Payload->TryGetArrayField(TEXT("moves"), RawMoveArray) || !RawMoveArray) {
    UE_LOG(MoveDBLog, Error, TEXT("Missing or invalid 'moves' array in payload"));
    return false;
  }
  OutData.MovesArray = *RawMoveArray;
  return true;
}
