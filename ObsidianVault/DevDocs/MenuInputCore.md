# Menu Input Core

## What to do

1. Inherit from `IMenuInputCore` in your menu widget
2. Override `NativeTick` and call `TriggerInputs(this)`
3. Override `ProcessInput(uint8 PlayerID, uint16 InputMask)` to handle input
4. Call `GameManager->SetGlobalInputMode(EInputMode::Menu)` when showing your menu

## What's available

- **Input bits**: `EMenuInputBit::Up`, `Down`, `Left`, `Right`, `Confirm`
- **Control**: `SetInputActive(false/true)` to enable/disable input processing
- **Multiple players**: `PlayerID` parameter tells you which player sent input

-----

## Boilerplate

```cpp
// MyMenuScreen.h
#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Interfaces/IMenuInputCore.h"
#include "MyMenuScreen.generated.h"

UCLASS()
class UMyMenuScreen : public UUserWidget, public IMenuInputCore {
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
    virtual void ProcessInput(uint8 PlayerID, uint16 InputMask) override;
};

// MyMenuScreen.cpp
#include "MyMenuScreen.h"
#include "Data/Structs/InputEnums.h"

void UMyMenuScreen::NativeTick(const FGeometry& MyGeometry, float InDeltaTime) {
    Super::NativeTick(MyGeometry, InDeltaTime);
    TriggerInputs(this);
}

void UMyMenuScreen::ProcessInput(uint8 PlayerID, uint16 InputMask) {
    if (InputMask & static_cast<uint16>(EMenuInputBit::Up)) {
        // Handle up
    }
    if (InputMask & static_cast<uint16>(EMenuInputBit::Confirm)) {
        // Handle confirm
    }
}
```

