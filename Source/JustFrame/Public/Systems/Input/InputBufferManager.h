// Every Frame is a Choice //
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InputBufferManager.generated.h"

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
  void Init(uint8 NumPlayers);
  void InjectInput(uint8 PlayerID, uint16 InputMask, uint32 Frame);
  void PushInput(uint8 PlayerID, uint16 InputMask, uint64 CurrentCycles);
  void Reset();

  const TArray<uint16> &GetInputsForFrame(uint32 Frame) const;
  int32 GetMaxFrame() const { return MaxFrame; }

private:
  bool bSimStarted = false;
  TMap<uint32, FInputFrame> InputHistory;
  int32 MaxFrame;
  uint8 PlayerCount;
};
