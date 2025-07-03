## TL;DR
- Inherit from `IMenuCore` in your menu widget (see `UI/Menu/IMenuCore.h`).
- Register your menu with `REGISTER_MENU_WIDGET(UMyMenu, TEXT("MenuName"));`.
- Input is routed automatically by `MenuManagerComponent`.
- Use `MenuManagerComponent` to open/close menus and manage input modes per player.

---

## Screen Implementation
- Inherit from `IMenuCore` in your menu widget class.
- Register your menu at the bottom of the header or cpp:
  ```cpp
  REGISTER_MENU_WIDGET(UMyMenu, TEXT("MenuName"));
  ```
- Implement `ProcessInput(uint8 PlayerID, uint16 InputMask)` to handle input.

---

## MenuManagerComponent Usage
- `OpenMenuForPlayer(PlayerID, MenuName, bShared, SharedPlayers)` — Open a menu for a player (or as a shared menu for multiple players).
- `CloseMenuForPlayer(PlayerID)` — Close the top menu for a player.
- `SetInputModeOnMenuClose(PlayerID, Mode)` — Set the input mode to use when a player's menu stack is empty.
- `TickMenu()` — Process input for all active menus (must be called after input polling).
- Input is always routed to the top menu for each player.

---

## Notes
- Shared menus can be opened for multiple players if their stacks are aligned.
- When a player's menu stack is empty, their input mode is set automatically.
- No need to call `TriggerInputs` or manage input activation in your menu widget.

```cpp
// MyMenuScreen.h
#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Menu/IMenuCore.h"
#include "UI/Menu/MenuFactory.h"
#include "MyMenuScreen.generated.h"

UCLASS()
class UMyMenuScreen : public UUserWidget, public IMenuCore {
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;
    virtual void ProcessInput(uint8 PlayerID, uint16 InputMask) override;
};

REGISTER_MENU_WIDGET(UMyMenuScreen, TEXT("MyMenu"));
```

```cpp
// MyMenuScreen.cpp
#include "UI/Menu/Screens/MyMenuScreen.h"
#include "Data/Structs/InputEnums.h"

void UMyMenuScreen::ProcessInput(uint8 PlayerID, uint16 InputMask) {
    if (InputMask & static_cast<uint16>(EMenuInputBit::Up)) {
        // Handle up
    }
    if (InputMask & static_cast<uint16>(EMenuInputBit::Confirm)) {
        // Handle confirm
    }
}
```

-----

## MenuManagerComponent API

- `OpenMenuForPlayer(uint8 PlayerID, const FString& MenuName, bool bShared = false, const std::set<uint8>& SharedPlayers = {})` — Open a menu for a player (or as a shared menu for multiple players)
- `CloseMenuForPlayer(uint8 PlayerID)` — Close the top menu for a player
- `SetInputModeOnMenuClose(uint8 PlayerID, EInputMode Mode)` — Set the input mode to use when a player's menu stack is empty
- `TickMenu()` — Process input for all active menus (must be called after input polling)
- Input is routed automatically to the top menu for each player

## Shared Menus
- Shared menus (e.g., character select) can be opened for multiple players if their stacks are aligned
- Closing a shared menu closes it for all involved players

## Input Mode Management
- The menu manager sets the correct input mode for each player when their menu stack is empty
- You can configure the fallback input mode per player using `SetInputModeOnMenuClose`

## Migration Notes
- Remove any usage of `TriggerInputs`, `SetInputActive`, or direct input polling from your menu widgets
- Use the new per-player stack and shared menu system for all menu navigation

