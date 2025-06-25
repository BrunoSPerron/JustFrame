// Every Frame is a Choice //
#pragma once

#include "CoreMinimal.h"
#include "Data/Structs/Structs_Character.h"

class IMoveDataSource {
public:
  virtual ~IMoveDataSource() = default;

  virtual bool LoadCharacterSources(const TArray<FString> &Collections, TArray<FMoveData> &OutMoves,
                                    TArray<FStanceData> &OutStances) = 0;

  virtual int32 GetCharacterSourceCount() const = 0;
  virtual FString GetCharacterSourceVersion(int32 FileIndex) const = 0;

protected:
  bool DeserializeCharacterPayloads(const TArray<FString> &RawJsonPayloads,
                                    TArray<FMoveData> &OutMoves, TArray<FStanceData> &OutStances);

  TArray<FString> CharacterSourceVersions;
};
