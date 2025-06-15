// Every Frame is a Choice
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Systems/Input/InputBufferManager.h"
#include "Characters/Base/RollbackCharacter.h"
#include "RollbackSimulationManager.generated.h"

USTRUCT()
struct FFrameSnapshot {
  GENERATED_BODY()

  uint32 FrameNumber;
  TArray<TArray<uint8>> SerializedStates; // Per-character state
};

UCLASS()
class JUSTFRAME_API URollbackSimulationManager : public UObject {
  GENERATED_BODY()

public:
  void Init(TArray<ARollbackCharacter *> InCharacters, UInputBufferManager *InInputBufferManager);
  void AdvanceSimulation();

  FORCEINLINE uint32 GetCurrentFrame() const { return CurrentFrame; }

  void InjectInput(uint8 PlayerID, uint16 InputMask);
  void RollbackTo(uint32 Frame);

private:
  TArray<ARollbackCharacter *> Characters;
  TMap<uint32, FFrameSnapshot> FrameSnapshots;

  uint32 CurrentFrame = 0;

  void SaveSnapshot(uint32 Frame);

  UPROPERTY()
  UInputBufferManager *InputBuffer;
};
