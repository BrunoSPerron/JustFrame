// Every Frame is a Choice //
#include "Systems/Input/InputBufferManager.h"
#include "Data/Consts.h"
#include "Systems/Player/PlayerSettingsManager.h"
#include "Kismet/GameplayStatics.h"

void UInputBufferManager::InjectInput(uint8 PlayerID, uint16 InputMask, uint32 Frame) {
  if (!PlayerSettingsManager) {
    UE_LOG(LogTemp, Error,
           TEXT("UInputBufferManager::InjectInput failed: PlayerSettingsManager is null"));
    return;
  }
  FInputFrame &InputFrame = InputHistory.FindOrAdd(Frame);
  InputFrame.FrameNumber = Frame;
  if (MaxFrame < Frame) MaxFrame = Frame;
  int32 NumPlayers = PlayerSettingsManager->GetNumPlayers();
  if (InputFrame.PlayerInputs.Num() < NumPlayers) InputFrame.PlayerInputs.SetNum(NumPlayers);
  if (PlayerID < NumPlayers) InputFrame.PlayerInputs[PlayerID] = InputMask;
}

void UInputBufferManager::Reset() {
  bSimStarted = false;
  InputHistory.Empty();
  MaxFrame = 0;
  CurrentUnbufferedInputs.Empty();
}

const TArray<uint16> &UInputBufferManager::GetInputsForFrame(uint32 Frame) const {
  static TArray<uint16> ZeroInputPlayerCount;
  if (!PlayerSettingsManager) {
    ZeroInputPlayerCount.SetNum(0);
    return ZeroInputPlayerCount;
  }
  int32 NumPlayers = PlayerSettingsManager->GetNumPlayers();
  const FInputFrame *Found = InputHistory.Find(Frame);
  if (Found) return Found->PlayerInputs;
  ZeroInputPlayerCount.SetNum(NumPlayers);
  for (int32 i = 0; i < NumPlayers; ++i) ZeroInputPlayerCount[i] = 0;
  return ZeroInputPlayerCount;
}

void UInputBufferManager::SetUnbufferedInput(uint8 PlayerID, uint16 InputMask) {
  if (!PlayerSettingsManager) {
    UE_LOG(LogTemp, Error,
           TEXT("UInputBufferManager::SetUnbufferedInput failed: PlayerSettingsManager is null"));
    return;
  }
  int32 NumPlayers = PlayerSettingsManager->GetNumPlayers();
  if (CurrentUnbufferedInputs.Num() < NumPlayers) CurrentUnbufferedInputs.SetNum(NumPlayers);
  if (PlayerID < NumPlayers) {
    CurrentUnbufferedInputs[PlayerID] = InputMask;
  }
}

uint16 UInputBufferManager::GetUnbufferedInput(uint8 PlayerID) const {
  if (!PlayerSettingsManager) {
    UE_LOG(LogTemp, Error,
           TEXT("UInputBufferManager::GetUnbufferedInput failed: PlayerSettingsManager is null"));
    return 0;
  }
  int32 NumPlayers = PlayerSettingsManager->GetNumPlayers();
  return (PlayerID < NumPlayers && CurrentUnbufferedInputs.Num() > PlayerID)
             ? CurrentUnbufferedInputs[PlayerID]
             : 0;
}
