// Every Frame is a Choice //
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Systems/Input/InputBufferManager.h"
#include "Characters/RollbackCharacter.h"
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
  const FCharacterState GetCharacterStateAtFrame(int32 Frame, uint8 CharacterIndex) const;

  void InjectInput(uint8 CharacterIndex, uint16 InputMask);
  void RollbackTo(uint32 Frame);

private:
  TArray<ARollbackCharacter *> Characters;
  uint32 CurrentFrame = 0;
  TMap<uint32, FFrameSnapshot> FrameSnapshots;
  UInputBufferManager *InputBuffer;

  void SaveSnapshot(uint32 Frame);
};
