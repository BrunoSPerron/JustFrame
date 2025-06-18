// Every Frame is a Choice //
#pragma once

#include "IMoveDataSource.h"

class FJsonFileListMoveDataSource : public IMoveDataSource {
public:
  bool LoadSources(const TArray<FString> &Collections) override;

  FString GetSourceName() const override;
  int32 GetSourceCount() const override { return FilePaths.Num(); }
  FString GetRawPayload(int32 FileIndex) const override;
  FString GetClaimedSignature(int32 FileIndex) const override;
  FString GetSourceVersion(int32 FileIndex) const override;

private:
  TArray<FString> FilePaths;
};
