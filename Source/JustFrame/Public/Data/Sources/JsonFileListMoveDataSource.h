// Every Frame is a Choice //
#pragma once

#include "CoreMinimal.h"
#include "Data/Sources/IMoveDataSource.h"

class FJsonFileListMoveDataSource : public IMoveDataSource {
public:
  virtual bool LoadCharacterSources(const TArray<FString> &Collections, TArray<FMoveData> &OutMoves,
                                    TArray<FStanceData> &OutStances) override;

  virtual int32 GetCharacterSourceCount() const override;
  virtual FString GetCharacterSourceVersion(int32 FileIndex) const override;

private:
  TArray<FString> CharacterFilePaths;
};
