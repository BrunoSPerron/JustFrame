// Every Frame is a Choice //
#pragma once

#include "CoreMinimal.h"
#include "Data/Structs_Move.h"

class IMoveDataSource {
public:
  virtual ~IMoveDataSource() = default;

  virtual bool LoadSources(const TArray<FString> &Collections) = 0;
  bool DeserializePayloads(TArray<FMoveData> &OutMoves);

  virtual FString GetSourceName() const = 0;
  virtual int32 GetSourceCount() const = 0;
  virtual FString GetRawPayload(int32 FileIndex) const = 0;
  virtual FString GetClaimedSignature(int32 FileIndex) const = 0;
  virtual FString GetSourceVersion(int32 FileIndex) const = 0;

protected:
  TArray<FString> CanonicalPayloadArray;
  TArray<FString> ClaimedSignatureArray;
  TArray<FString> SourceVersions;
};
