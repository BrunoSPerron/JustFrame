// Every Frame is a Choice //
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InputBufferManager.generated.h"

class UPlayerSettingsManager;

USTRUCT()
struct FInputFrame {
  GENERATED_BODY()

  uint32 FrameNumber;
  TArray<uint16> PlayerInputs;
};

UCLASS()
class JUSTFRAME_API UInputBufferManager : public UObject {
  GENERATED_BODY()

public:
  void Init(UPlayerSettingsManager *InManager) { PlayerSettingsManager = InManager; }
  void InjectInput(uint8 PlayerID, uint16 InputMask, uint32 Frame);
  void PushInput(uint8 PlayerID, uint16 InputMask, uint64 CurrentCycles);
  void Reset();

  uint16 GetUnbufferedInput(uint8 PlayerID) const;
  void SetUnbufferedInput(uint8 PlayerID, uint16 InputMask);

  const TArray<uint16> &GetInputsForFrame(uint32 Frame) const;
  int32 GetMaxFrame() const { return MaxFrame; }

private:
  TArray<uint16> CurrentUnbufferedInputs;
  bool bSimStarted = false;
  TMap<uint32, FInputFrame> InputHistory;
  int32 MaxFrame;
  UPlayerSettingsManager *PlayerSettingsManager = nullptr;
};
