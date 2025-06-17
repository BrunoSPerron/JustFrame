// Every Frame is a Choice //
#pragma once

#include "CoreMinimal.h"
#include "Data/Structs_Character.h"

class IMoveDataSource {
public:
  virtual ~IMoveDataSource() = default;

  virtual bool LoadSources(const TArray<FString> &Collections) = 0;
  virtual bool ParseMoves(TArray<FMoveData> &OutMoves) = 0;

  virtual FString GetSourceName() const = 0;
  virtual int32 GetSourceCount() const = 0;
  virtual FString GetRawMovesSection(int32 FileIndex) const = 0;
  virtual FString GetClaimedSignature(int32 FileIndex) const = 0;
};
