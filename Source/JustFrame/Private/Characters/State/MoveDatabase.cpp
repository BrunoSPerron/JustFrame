// Every Frame is a Choice //
#include "Characters/State/MoveDatabase.h"
#include "JsonObjectConverter.h"
#include "Characters/State/JsonFileListMoveDataSource.h"
#include "Data/LogCategories.h"
#include "Data/Consts.h"
#include "Data/Enums_Character.h"
#include "Utilities/CryptoUtils.h"

void UMoveDatabase::LoadMoves(const TArray<FString> &Collections) {
  DataSource = MakeUnique<FJsonFileListMoveDataSource>();
  DataSource->LoadSources(Collections);

  for (int32 SourceIndex = 0; SourceIndex < DataSource->GetSourceCount(); ++SourceIndex) {
    const FString RawPayload = DataSource->GetRawPayload(SourceIndex);
    const FString Signature = DataSource->GetClaimedSignature(SourceIndex);

    if (!FCryptoUtils::VerifySignatureECDSA(MOVE_DATA_PUBLIC_KEY_PEM, RawPayload, Signature)) {
      UE_LOG(MoveDBLog, Warning, TEXT("Signature verification failed for file %d in source: %s"),
             SourceIndex, *DataSource->GetSourceName());
      // TODO -> Something with that
      // -Add a label in local play
      // -Download valid values when online
      // return;
    }
  }

  TArray<FMoveData> LoadedMoves;
  if (!DataSource->DeserializePayloads(LoadedMoves)) {
    UE_LOG(MoveDBLog, Error, TEXT("Move loading failed from source: %s"),
           *DataSource->GetSourceName());
    return;
  }

  MoveMap.Empty();
  for (const FMoveData &Move : LoadedMoves) MoveMap.Add(Move.MoveID, Move);

  UE_LOG(MoveDBLog, Log, TEXT("Loaded %d moves from %s"), MoveMap.Num(),
         *DataSource->GetSourceName());

  const FMoveData *Punch = FindMove("SimplePunch");
  TSharedPtr<FJsonObject> JsonObject = FJsonObjectConverter::UStructToJsonObject(*Punch);
  FString PrettyJson;
  TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&PrettyJson, 0);
  if (JsonObject.IsValid()) {
    FString PrettyJson;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&PrettyJson, 0);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
    UE_LOG(MoveDBLog, Log, TEXT("Punch:\n%s"), *PrettyJson);
  }
}

const FMoveData *UMoveDatabase::FindMove(FName MoveID) const { return MoveMap.Find(MoveID); }
