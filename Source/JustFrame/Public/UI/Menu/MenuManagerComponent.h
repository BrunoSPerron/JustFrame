#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UI/Menu/IMenuCore.h"
#include <map>
#include <vector>
#include <set>
#include "MenuManagerComponent.generated.h"

class UUserWidget;
class UGameManagerSubsystem;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class JUSTFRAME_API UMenuManagerComponent : public UActorComponent {
  GENERATED_BODY()
public:
  UMenuManagerComponent();

  bool OpenMenuForPlayer(uint8 PlayerID, const FString &MenuName, bool bShared = false,
                         const std::set<uint8> &SharedPlayers = {});
  void CloseMenuForPlayer(uint8 PlayerID);
  IMenuCore *GetTopMenuForPlayer(uint8 PlayerID) const;
  void SetInputModeOnMenuClose(uint8 PlayerID, EInputMode Mode);
  void TickMenu();

protected:
  virtual void BeginPlay() override;

private:
  UGameManagerSubsystem *GameManager = nullptr;
  std::map<uint8, std::vector<IMenuCore *>> PlayerMenuStacks;
  std::map<IMenuCore *, UUserWidget *> MenuToWidget;
  std::map<uint8, EInputMode> InputModeOnMenuClose;

  bool CanOpenSharedMenu(const std::set<uint8> &SharedPlayers) const;
  void PopSharedMenu(IMenuCore *Menu);
};