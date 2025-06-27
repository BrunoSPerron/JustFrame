// Every Frame is a Choice //
#include "Utilities/Deserializer.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Dom/JsonObject.h"
#include "Dom/JsonValue.h"
#include "JsonObjectConverter.h"
#include "Data/LogCategories.h"

bool FDeserializer::DeserializeCharacterData(const FString &JsonString,
                                             FDeserializedCharacterData &OutData) {
  TSharedPtr<FJsonObject> Root;
  TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
  if (!FJsonSerializer::Deserialize(Reader, Root) || !Root.IsValid()) {
    UE_LOG(CharacterDBLog, Error,
           TEXT("FDeserializer::DeserializeCharacterData Invalid JSON string for character data"));
    return false;
  }

  if (!Root->TryGetStringField(TEXT("version"), OutData.Version) || OutData.Version.IsEmpty()) {
    UE_LOG(
        CharacterDBLog, Error,
        TEXT("FDeserializer::DeserializeCharacterData Missing 'version' field in character JSON"));
    return false;
  }

  ParseMoveArray(Root, OutData.Moves);
  ParseStanceArray(Root, OutData.Stances);

  return true;
}

bool FDeserializer::DeserializeCharacterList(const FString &JsonString,
                                             TArray<FCharacterListEntry> &OutList) {
  TArray<TSharedPtr<FJsonValue>> RawArray;
  const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

  if (!FJsonSerializer::Deserialize(Reader, RawArray)) {
    UE_LOG(
        LogTemp, Error,
        TEXT("FDeserializer::DeserializeCharacterList Character list JSON is not a valid array"));
    return false;
  }

  for (const TSharedPtr<FJsonValue> &Value : RawArray) {
    const TSharedPtr<FJsonObject> Object = Value->AsObject();
    if (!Object.IsValid()) {
      UE_LOG(LogTemp, Warning,
             TEXT("FDeserializer::DeserializeCharacterList Skipping invalid character list entry "
                  "(not an object)"));
      continue;
    }

    FCharacterListEntry Entry;
    if (!FJsonObjectConverter::JsonObjectToUStruct(
            Object.ToSharedRef(), FCharacterListEntry::StaticStruct(), &Entry, 0, 0)) {
      UE_LOG(LogTemp, Warning,
             TEXT("FDeserializer::DeserializeCharacterList Failed to parse character list entry"));
      continue;
    }

    OutList.Add(MoveTemp(Entry));
  }

  return true;
}

bool FDeserializer::DeserializeManifest(const FString &Payload, FManifestData &OutManifest) {
  TSharedPtr<FJsonObject> Root;
  const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Payload);

  if (!FJsonSerializer::Deserialize(Reader, Root) || !Root.IsValid()) {
    UE_LOG(
        LogTemp, Error,
        TEXT("FDeserializer::DeserializeManifest Manifest deserialization failed: Invalid JSON"));
    return false;
  }

  if (!Root->TryGetStringField(TEXT("signature"), OutManifest.Signature) ||
      OutManifest.Signature.IsEmpty()) {
    UE_LOG(
        LogTemp, Error,
        TEXT("FDeserializer::DeserializeManifest Manifest missing or invalid 'signature' field"));
    return false;
  }

  const TSharedPtr<FJsonObject> *PayloadObj;
  if (!Root->TryGetObjectField(TEXT("payload"), PayloadObj) || !PayloadObj ||
      !PayloadObj->IsValid()) {
    UE_LOG(LogTemp, Error,
           TEXT("FDeserializer::DeserializeManifest Manifest missing or invalid 'payload' object"));
    return false;
  }

  for (const auto &Pair : (*PayloadObj)->Values) {
    const FString &Path = Pair.Key;
    const TSharedPtr<FJsonObject> EntryObj = Pair.Value->AsObject();
    if (!EntryObj.IsValid()) {
      UE_LOG(LogTemp, Warning,
             TEXT("FDeserializer::DeserializeManifest Skipping malformed manifest entry: %s"),
             *Path);
      continue;
    }

    FManifestEntry Entry;
    if (!EntryObj->TryGetStringField(TEXT("hash"), Entry.Hash) || Entry.Hash.IsEmpty()) {
      UE_LOG(LogTemp, Warning,
             TEXT("FDeserializer::DeserializeManifest Skipping entry with missing 'hash': %s"),
             *Path);
      continue;
    }

    EntryObj->TryGetStringField(TEXT("version"), Entry.Version);
    if (Entry.Version.IsEmpty()) Entry.Version = TEXT("1.0");

    OutManifest.Entries.Add(Path, Entry);
  }

  return true;
}

void FDeserializer::ParseMoveArray(const TSharedPtr<FJsonObject> &JsonObject,
                                   TArray<FMoveData> &OutMoves) {
  const TArray<TSharedPtr<FJsonValue>> *RawArray = nullptr;
  if (!JsonObject->TryGetArrayField(TEXT("moves"), RawArray) || !RawArray) {
    UE_LOG(CharacterDBLog, Warning,
           TEXT("FDeserializer::ParseMoveArray No 'moves' array found in character data"));
    return;
  }

  for (const TSharedPtr<FJsonValue> &Value : *RawArray) {
    const TSharedPtr<FJsonObject> Object = Value->AsObject();
    if (!Object.IsValid()) {
      UE_LOG(CharacterDBLog, Warning,
             TEXT("FDeserializer::ParseMoveArray Invalid move entry (not an object)"));
      continue;
    }

    FMoveData Parsed;
    if (!FJsonObjectConverter::JsonObjectToUStruct(Object.ToSharedRef(), FMoveData::StaticStruct(),
                                                   &Parsed, 0, 0)) {
      UE_LOG(CharacterDBLog, Warning,
             TEXT("FDeserializer::ParseMoveArray Failed to parse move entry to struct"));
      continue;
    }

    OutMoves.Add(Parsed);
  }
}

void FDeserializer::ParseStanceArray(const TSharedPtr<FJsonObject> &JsonObject,
                                     TArray<FStanceData> &OutStances) {
  const TArray<TSharedPtr<FJsonValue>> *RawArray = nullptr;
  if (!JsonObject->TryGetArrayField(TEXT("stances"), RawArray) || !RawArray) {
    UE_LOG(CharacterDBLog, Warning,
           TEXT("FDeserializer::ParseStanceArray No 'stances' array found in character data"));
    return;
  }

  for (const TSharedPtr<FJsonValue> &Value : *RawArray) {
    const TSharedPtr<FJsonObject> Object = Value->AsObject();
    if (!Object.IsValid()) {
      UE_LOG(CharacterDBLog, Warning,
             TEXT("FDeserializer::ParseStanceArray Invalid stance entry (not an object)"));
      continue;
    }

    FStanceData Parsed;
    if (!FJsonObjectConverter::JsonObjectToUStruct(Object.ToSharedRef(),
                                                   FStanceData::StaticStruct(), &Parsed, 0, 0)) {
      UE_LOG(CharacterDBLog, Warning,
             TEXT("FDeserializer::ParseStanceArray Failed to parse stance entry to struct"));
      continue;
    }

    OutStances.Add(Parsed);
  }
}
