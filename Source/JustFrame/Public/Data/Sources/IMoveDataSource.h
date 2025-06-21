// Every Frame is a Choice //
#pragma once

#include "CoreMinimal.h"
#include "Data/Structs/Structs_Move.h"

class IMoveDataSource {
public:
  virtual ~IMoveDataSource() = default;

  virtual bool LoadMoveSources(const TArray<FString> &Collections) = 0;
  virtual bool LoadStanceSources(const TArray<FString> &Collections) = 0;

  bool DeserializeMovePayloads(TArray<FMoveData> &OutMoves);
  bool DeserializeStancePayloads(TArray<FNeutralStanceData> &OutStances);

  // Move-specific accessors
  virtual FString GetMoveSourceName() const = 0;
  virtual int32 GetMoveSourceCount() const = 0;
  virtual FString GetMoveRawPayload(int32 FileIndex) const = 0;
  virtual FString GetMoveClaimedSignature(int32 FileIndex) const = 0;
  virtual FString GetMoveSourceVersion(int32 FileIndex) const = 0;

  // Stance-specific accessors
  virtual FString GetStanceSourceName() const = 0;
  virtual int32 GetStanceSourceCount() const = 0;
  virtual FString GetStanceRawPayload(int32 FileIndex) const = 0;
  virtual FString GetStanceClaimedSignature(int32 FileIndex) const = 0;
  virtual FString GetStanceSourceVersion(int32 FileIndex) const = 0;

protected:
  // Move payloads
  TArray<FString> CanonicalMovePayloadArray;
  TArray<FString> ClaimedMoveSignatureArray;
  TArray<FString> MoveSourceVersions;

  // Stance payloads
  TArray<FString> CanonicalStancePayloadArray;
  TArray<FString> ClaimedStanceSignatureArray;
  TArray<FString> StanceSourceVersions;
};
