// Every Frame is a Choice //

#pragma once

#include "CoreMinimal.h"

struct FManifestEntry {
  FString Hash;
  FString Version;
};

struct FManifestData {
  TMap<FString, FManifestEntry> Entries;
  FString Signature;
};