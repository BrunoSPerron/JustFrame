// Every Frame is a Choice //
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/Structs/CharacterStructs.h"
#include "Components/UniformGridPanel.h"
#include "Components/SizeBox.h"
#include "Components/Image.h"
#include "UI/Menu/IMenuCore.h"
#include "UI/Menu/MenuRegistration.h"
#include "CharacterSelectScreen.generated.h"

UCLASS()
class UCharacterSelectScreen : public UUserWidget, public IMenuCore {
  GENERATED_BODY()

public:
  virtual void NativeConstruct() override;
  virtual void NativeTick(const FGeometry &MyGeometry, float InDeltaTime) override;

  void MoveCursor(uint8 PlayerID, int32 Direction);
  void ConfirmSelection(uint8 PlayerID);

  virtual void ProcessInput(uint8 PlayerID, uint16 InputMask) override;

protected:
  UPROPERTY(meta = (BindWidget))
  UUniformGridPanel *CharacterGrid;

  UPROPERTY(EditDefaultsOnly, Category = "UI")
  UTexture2D *SelectionBorderTexture;

private:
  static constexpr float CardHeight = 256.f;
  static constexpr float CardWidth = CardHeight * 5.f / 7.f;
  static constexpr int32 Columns = 4;

  TArray<FCharacterListEntry> CharacterList;
  TMap<FString, UTexture2D *> CharacterCards;
  TArray<USizeBox *> CardBoxes;
  TMap<uint8, int32> PlayerSelections;

  void PopulateGrid();
  void UpdateSelection(uint8 PlayerID);
};

REGISTER_MENU_WIDGET(UCharacterSelectScreen, TEXT("CharacterSelect"));
