#pragma once
#include "CoreMinimal.h"
#include "UObject/UObjectGlobals.h"
#include "Blueprint/UserWidget.h"

class UMenuFactory {
public:
  static void Register(const FString &Name, TSubclassOf<UUserWidget> MenuClass);
  static TSubclassOf<UUserWidget> Find(const FString &Name);
};