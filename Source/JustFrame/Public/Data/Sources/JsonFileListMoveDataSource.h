// Every Frame is a Choice //
#pragma once

#include "CoreMinimal.h"
#include "Data/Sources/IMoveDataSource.h"

class FJsonFileListMoveDataSource : public IMoveDataSource {
public:
  virtual ~FJsonFileListMoveDataSource() override = default;

  // Separate loaders for each payload type
  virtual bool LoadMoveSources(const TArray<FString> &Collections) override;
  virtual bool LoadStanceSources(const TArray<FString> &Collections) override;

  // Move access
  virtual FString GetMoveRawPayload(int32 FileIndex) const override;
  virtual FString GetMoveClaimedSignature(int32 FileIndex) const override;
  virtual FString GetMoveSourceVersion(int32 FileIndex) const override;
  virtual int32 GetMoveSourceCount() const override;
  virtual FString GetMoveSourceName() const override;

  // Stance access
  virtual FString GetStanceRawPayload(int32 FileIndex) const override;
  virtual FString GetStanceClaimedSignature(int32 FileIndex) const override;
  virtual FString GetStanceSourceVersion(int32 FileIndex) const override;
  virtual int32 GetStanceSourceCount() const override;
  virtual FString GetStanceSourceName() const override;

private:
  TArray<FString> MoveFilePaths;
  TArray<FString> StanceFilePaths;

  // Move payloads
  TArray<FString> CanonicalMovePayloadArray;
  TArray<FString> ClaimedMoveSignatureArray;
  TArray<FString> MoveSourceVersions;

  // Stance payloads
  TArray<FString> CanonicalStancePayloadArray;
  TArray<FString> ClaimedStanceSignatureArray;
  TArray<FString> StanceSourceVersions;
};
