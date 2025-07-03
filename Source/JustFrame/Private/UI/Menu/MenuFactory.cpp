#include "UI/Menu/MenuFactory.h"
#include "Blueprint/UserWidget.h"

static TMap<FString, TSubclassOf<UUserWidget>> &GetMenuRegistry() {
  static TMap<FString, TSubclassOf<UUserWidget>> Registry;
  return Registry;
}

void UMenuFactory::Register(const FString &Name, TSubclassOf<UUserWidget> MenuClass) {
  GetMenuRegistry().Add(Name, MenuClass);
}

TSubclassOf<UUserWidget> UMenuFactory::Find(const FString &Name) {
  return GetMenuRegistry().FindRef(Name);
}