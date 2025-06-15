// Every Frame is a Choice
#include "Systems/Input/InputBufferManager.h"
#include "Data/Consts.h"
#include "Kismet/GameplayStatics.h"

void UInputBufferManager::Init(uint8 NumPlayers) {
  PlayerCount = NumPlayers;
  InputHistory.Empty();
}

void UInputBufferManager::InjectInput(uint8 PlayerID, uint16 InputMask, uint32 Frame) {
  FInputFrame &InputFrame = InputHistory.FindOrAdd(Frame);
  InputFrame.FrameNumber = Frame;

  if (InputFrame.PlayerInputs.Num() < PlayerCount) InputFrame.PlayerInputs.SetNum(PlayerCount);

  InputFrame.PlayerInputs[PlayerID] = InputMask;

  if (MaxFrame < Frame) MaxFrame = Frame;
}

void UInputBufferManager::Reset() {
  bSimStarted = false;
  SimStartCycles = 0;
  InputHistory.Empty();
  MaxFrame = 0;
}

const TArray<uint16> &UInputBufferManager::GetInputsForFrame(uint32 Frame) const {
  const FInputFrame *Found = InputHistory.Find(Frame);
  if (Found) {
    return Found->PlayerInputs;
  }
  static TArray<uint16> ZeroInputPlayerCount;
  ZeroInputPlayerCount.Init(0, PlayerCount);
  return ZeroInputPlayerCount;
}
