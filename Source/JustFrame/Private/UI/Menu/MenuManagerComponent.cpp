#include "UI/Menu/MenuManagerComponent.h"
#include "Blueprint/UserWidget.h"
#include "Engine/World.h"
#include "Game/GameManagerSubsystem.h"
#include "Systems/Input/InputRouter.h"
#include "Systems/Input/InputPollingService.h"
#include "UI/Menu/MenuFactory.h"

UMenuManagerComponent::UMenuManagerComponent() { PrimaryComponentTick.bCanEverTick = true; }

void UMenuManagerComponent::BeginPlay() {
  Super::BeginPlay();
  if (!GameManager && GetWorld()) {
    if (UGameInstance *GameInstance = GetWorld()->GetGameInstance()) {
      GameManager = GameInstance->GetSubsystem<UGameManagerSubsystem>();
    }
  }
}

bool UMenuManagerComponent::CanOpenSharedMenu(const std::set<uint8> &SharedPlayers) const {
  if (SharedPlayers.empty()) return false;
  // All stacks must be the same depth and top menu (if any) must not be a single-player menu
  int expectedDepth = -1;
  for (uint8 PlayerID : SharedPlayers) {
    auto it = PlayerMenuStacks.find(PlayerID);
    int depth = (it != PlayerMenuStacks.end()) ? static_cast<int>(it->second.size()) : 0;
    if (expectedDepth == -1) expectedDepth = depth;
    if (depth != expectedDepth) return false;
    if (depth > 0) {
      IMenuCore *top = it->second.back();
      if (top->GetActivePlayers().Num() == 1) return false; // single-player menu on top
    }
  }
  return true;
}

bool UMenuManagerComponent::OpenMenuForPlayer(uint8 PlayerID, const FString &MenuName, bool bShared,
                                              const std::set<uint8> &SharedPlayers) {
  UE_LOG(LogTemp, Log,
         TEXT("UMenuManagerComponent::OpenMenuForPlayer Player %d, Menu: %s, Shared: %d"), PlayerID,
         *MenuName, bShared);

  if (bShared) {
    std::set<uint8> Players = SharedPlayers;
    Players.insert(PlayerID);
    if (!CanOpenSharedMenu(Players)) {
      UE_LOG(LogTemp, Warning,
             TEXT("UMenuManagerComponent::OpenMenuForPlayer CanOpenSharedMenu failed"));
      return false;
    }
    TSubclassOf<UUserWidget> MenuClass = UMenuFactory::Find(MenuName);
    UE_LOG(LogTemp, Log, TEXT("UMenuManagerComponent::OpenMenuForPlayer MenuClass: %p"),
           MenuClass.Get());

    if (!MenuClass || !GetWorld()) {
      UE_LOG(LogTemp, Warning,
             TEXT("UMenuManagerComponent::OpenMenuForPlayer MenuClass or World is null"));
      return false;
    }
    UUserWidget *Widget = CreateWidget<UUserWidget>(GetWorld(), MenuClass);
    UE_LOG(LogTemp, Log, TEXT("UMenuManagerComponent::OpenMenuForPlayer Widget created: %p"),
           Widget);

    if (!Widget) return false;
    IMenuCore *Menu = Cast<IMenuCore>(Widget);
    if (!Menu) {
      UE_LOG(LogTemp, Warning,
             TEXT("UMenuManagerComponent::OpenMenuForPlayer Widget doesn't implement IMenuCore"));
      Widget->RemoveFromParent();
      return false;
    }
    TSet<uint8> ActivePlayers;
    for (uint8 P : Players) {
      ActivePlayers.Add(P);
    }
    Menu->SetActivePlayers(ActivePlayers);
    MenuToWidget[Menu] = Widget;
    Widget->AddToViewport();
    for (uint8 P : Players) PlayerMenuStacks[P].push_back(Menu);

    UE_LOG(LogTemp, Log,
           TEXT("UMenuManagerComponent::OpenMenuForPlayer Successfully opened shared menu for %d "
                "players"),
           Players.size());
    return true;
  } else {
    // Single-player menu: just push for this player
    TSubclassOf<UUserWidget> MenuClass = UMenuFactory::Find(MenuName);
    if (!MenuClass || !GetWorld()) return false;
    UUserWidget *Widget = CreateWidget<UUserWidget>(GetWorld(), MenuClass);
    if (!Widget) return false;
    IMenuCore *Menu = Cast<IMenuCore>(Widget);
    if (!Menu) {
      Widget->RemoveFromParent();
      return false;
    }
    TSet<uint8> SinglePlayerSet;
    SinglePlayerSet.Add(PlayerID);
    Menu->SetActivePlayers(SinglePlayerSet);
    MenuToWidget[Menu] = Widget;
    Widget->AddToViewport();
    PlayerMenuStacks[PlayerID].push_back(Menu);
    return true;
  }
}

void UMenuManagerComponent::PopSharedMenu(IMenuCore *Menu) {
  const TSet<uint8> &Players = Menu->GetActivePlayers();
  for (uint8 PlayerID : Players) {
    auto &stack = PlayerMenuStacks[PlayerID];
    if (!stack.empty() && stack.back() == Menu) {
      stack.pop_back();
      if (stack.empty() && GameManager) {
        auto ModeIt = InputModeOnMenuClose.find(PlayerID);
        if (ModeIt != InputModeOnMenuClose.end()) {
          GameManager->SetInputMode(PlayerID, ModeIt->second);
        }
      }
    }
  }
  auto WidgetIt = MenuToWidget.find(Menu);
  if (WidgetIt != MenuToWidget.end()) {
    if (WidgetIt->second) WidgetIt->second->RemoveFromParent();
    MenuToWidget.erase(WidgetIt);
  }
}

void UMenuManagerComponent::CloseMenuForPlayer(uint8 PlayerID) {
  auto &stack = PlayerMenuStacks[PlayerID];
  if (stack.empty()) return;
  IMenuCore *top = stack.back();
  const TSet<uint8> &Players = top->GetActivePlayers();
  if (Players.Num() > 1) {
    // Shared menu: pop for all involved players
    PopSharedMenu(top);
  } else {
    // Single-player menu: pop for this player only
    stack.pop_back();
    auto WidgetIt = MenuToWidget.find(top);
    if (WidgetIt != MenuToWidget.end()) {
      if (WidgetIt->second) WidgetIt->second->RemoveFromParent();
      MenuToWidget.erase(WidgetIt);
    }
    if (stack.empty() && GameManager) {
      auto ModeIt = InputModeOnMenuClose.find(PlayerID);
      if (ModeIt != InputModeOnMenuClose.end()) {
        GameManager->SetInputMode(PlayerID, ModeIt->second);
      }
    }
  }
}

void UMenuManagerComponent::TickMenu() {
  if (!GameManager) return;
  UInputBufferManager *Buffer = GameManager->GetInputBufferManager();
  UPlayerSettingsManager *PlayerSettings = GameManager->GetPlayerSettingsManager();
  if (!Buffer || !PlayerSettings) return;

  TArray<uint8> PlayerIDs = PlayerSettings->GetActivePlayerIDs();
  for (uint8 PlayerID : PlayerIDs) {
    IMenuCore *Menu = GetTopMenuForPlayer(PlayerID);
    if (Menu) {
      const uint16 InputMask = Buffer->GetUnbufferedInput(PlayerID);
      if (InputMask != 0) {
        UE_LOG(LogTemp, Log,
               TEXT("UMenuManagerComponent::TickMenu processing input for Player %d, "
                    "InputMask: %d"),
               PlayerID, InputMask);
        Menu->ProcessInput(PlayerID, InputMask);
      }
    }
  }
}

IMenuCore *UMenuManagerComponent::GetTopMenuForPlayer(uint8 PlayerID) const {
  auto it = PlayerMenuStacks.find(PlayerID);
  if (it != PlayerMenuStacks.end() && !it->second.empty()) {
    return it->second.back();
  }
  return nullptr;
}

void UMenuManagerComponent::SetInputModeOnMenuClose(uint8 PlayerID, EInputMode Mode) {
  InputModeOnMenuClose[PlayerID] = Mode;
  auto it = PlayerMenuStacks.find(PlayerID);
  bool isEmpty = (it == PlayerMenuStacks.end() || it->second.empty());
  if (isEmpty && GameManager) {
    GameManager->SetInputMode(PlayerID, Mode);
  }
}