// Every Frame is a Choice //
#pragma once

#include "CoreMinimal.h"
#include "Data/Structs/Structs_Character.h"

class IMoveDataSource {
public:
  virtual ~IMoveDataSource() = default;

  virtual bool LoadCharacterSources(const TArray<FString> &Collections) = 0;
  bool DeserializeCharacterPayloads(TArray<FMoveData> &OutMoves, TArray<FStanceData> &OutStances);

  virtual FString GetCharacterSourceName() const = 0;
  virtual int32 GetCharacterSourceCount() const = 0;
  virtual FString GetCharacterRawPayload(int32 FileIndex) const = 0;
  virtual FString GetCharacterClaimedSignature(int32 FileIndex) const = 0;
  virtual FString GetCharacterSourceVersion(int32 FileIndex) const = 0;

protected:
  TArray<FString> CanonicalCharacterPayloadArray;
  TArray<FString> ClaimedCharacterSignatureArray;
  TArray<FString> CharacterSourceVersions;
};
