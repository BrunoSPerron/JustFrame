// Every Frame is a Choice //
#pragma once

#include "CoreMinimal.h"
#include "Data/Sources/IMoveDataSource.h"

class FJsonFileListMoveDataSource : public IMoveDataSource {
public:
  virtual bool LoadCharacterSources(const TArray<FString> &Collections) override;

  virtual FString GetCharacterSourceName() const override;
  virtual int32 GetCharacterSourceCount() const override;
  virtual FString GetCharacterRawPayload(int32 FileIndex) const override;
  virtual FString GetCharacterClaimedSignature(int32 FileIndex) const override;
  virtual FString GetCharacterSourceVersion(int32 FileIndex) const override;

private:
  TArray<FString> CharacterFilePaths;
};
