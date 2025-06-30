// Every Frame is a Choice //
#include "UI/CharacterSelectScreen.h"
#include "Data/Structs/InputEnums.h"
#include "Utilities/ExternalDataLoader.h"

#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Blueprint/WidgetTree.h"

void UCharacterSelectScreen::NativeConstruct() {
  Super::NativeConstruct();

  ExternalDataLoader::LoadCharacterList(CharacterList);
  ExternalDataLoader::LoadCharacterCards(CharacterList, CharacterCards);

  PopulateGrid();

  // TODO: Logic to handle more than 2 players?
  // Maybe use the first 2 players hitting a button?
  if (UGameInstance *GameInstance = GetGameInstance()) {
    if (UPlayerSettingsManager *PlayerSettings =
            GameInstance->GetSubsystem<UGameManagerSubsystem>()->GetPlayerSettingsManager()) {
      TArray<uint8> PlayerIDs = PlayerSettings->GetActivePlayerIDs();
      for (uint8 PlayerID : PlayerIDs) {
        PlayerSelections.Add(PlayerID, PlayerID);
        UpdateSelection(PlayerID);
      }
    }
  }
}

void UCharacterSelectScreen::ConfirmSelection(uint8 PlayerID) {
  int32 Index = PlayerSelections.FindRef(PlayerID);
  if (CharacterList.IsValidIndex(Index)) {
    const FString &SelectedId = CharacterList[Index].Id;
    // TODO Trigger something — log, notify game mode, etc.
    UE_LOG(LogTemp, Log, TEXT("Player %d selected character: %s"), PlayerID, *SelectedId);
  }
}

void UCharacterSelectScreen::MoveCursor(uint8 PlayerID, int32 Direction) {
  if (!PlayerSelections.Contains(PlayerID)) return;

  int32 NewIndex = PlayerSelections[PlayerID] + Direction;

  if (NewIndex < 0) NewIndex = 0;
  if (NewIndex >= CardBoxes.Num()) NewIndex = CardBoxes.Num() - 1;

  PlayerSelections[PlayerID] = NewIndex;
  UpdateSelection(PlayerID);
}

void UCharacterSelectScreen::ProcessInput(uint8 PlayerID, uint16 InputMask) {
  UE_LOG(LogTemp, Log,
         TEXT("CharacterSelectScreen::ProcessInput called for Player %d with InputMask %d"),
         PlayerID, InputMask);
  const uint16 Up = static_cast<uint16>(EMenuInputBit::Up);
  const uint16 Down = static_cast<uint16>(EMenuInputBit::Down);
  const uint16 Left = static_cast<uint16>(EMenuInputBit::Left);
  const uint16 Right = static_cast<uint16>(EMenuInputBit::Right);
  const uint16 Confirm = static_cast<uint16>(EMenuInputBit::Confirm);

  int32 Direction = 0;
  if (InputMask & Up) Direction -= Columns;
  if (InputMask & Down) Direction += Columns;
  if (InputMask & Left) Direction -= 1;
  if (InputMask & Right) Direction += 1;

  if (Direction != 0) MoveCursor(PlayerID, Direction);

  if (InputMask & Confirm) ConfirmSelection(PlayerID);
}

void UCharacterSelectScreen::PopulateGrid() {
  if (!CharacterGrid || !WidgetTree) return;

  CharacterGrid->ClearChildren();
  CardBoxes.Empty();

  int32 Row = 0;
  int32 Col = 0;

  for (const FCharacterListEntry &Entry : CharacterList) {
    UTexture2D *Texture = CharacterCards.FindRef(Entry.Id);
    if (!Texture) continue;

    USizeBox *CardBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
    CardBox->SetWidthOverride(CardWidth);
    CardBox->SetHeightOverride(CardHeight);

    UOverlay *Overlay = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass());

    UImage *SelectionHighlight = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
    SelectionHighlight->SetColorAndOpacity(FLinearColor::Transparent);
    SelectionHighlight->SetDesiredSizeOverride(FVector2D(CardWidth, CardHeight));
    if (SelectionBorderTexture) {
      SelectionHighlight->SetBrushFromTexture(SelectionBorderTexture);
    }

    UImage *CardImage = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
    CardImage->SetBrushFromTexture(Texture);

    Overlay->AddChildToOverlay(SelectionHighlight);
    Overlay->AddChildToOverlay(CardImage);
    CardBox->AddChild(Overlay);

    CharacterGrid->AddChildToUniformGrid(CardBox, Row, Col);
    CardBoxes.Add(CardBox);

    if (++Col >= Columns) {
      Col = 0;
      ++Row;
    }
  }
}

void UCharacterSelectScreen::UpdateSelection(uint8 PlayerID) {
  for (int32 i = 0; i < CardBoxes.Num(); ++i) {
    USizeBox *Box = CardBoxes[i];
    if (!Box || Box->GetChildrenCount() == 0) continue;

    UOverlay *Overlay = Cast<UOverlay>(Box->GetChildAt(0));
    if (!Overlay || Overlay->GetChildrenCount() == 0) continue;

    UImage *SelectionImage = Cast<UImage>(Overlay->GetChildAt(0));
    if (!SelectionImage) continue;

    bool bSelected = false;
    for (const auto &[PID, Index] : PlayerSelections) {
      if (Index == i && PID == PlayerID) {
        bSelected = true;
        break;
      }
    }

    SelectionImage->SetColorAndOpacity(bSelected ? FLinearColor::Yellow
                                                 : FLinearColor::Transparent);
  }
}

void UCharacterSelectScreen::NativeTick(const FGeometry &MyGeometry, float InDeltaTime) {
  Super::NativeTick(MyGeometry, InDeltaTime);
  TriggerInputs(this);
}
