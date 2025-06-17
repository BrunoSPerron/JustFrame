// Every Frame is a Choice //
#include "Systems/Rollback/RollbackSimulationManager.h"
#include "Characters/Base/RollbackCharacter.h"
#include "Data/Consts.h"
#include "Kismet/GameplayStatics.h"

void URollbackSimulationManager::Init(TArray<ARollbackCharacter *> InCharacters,
                                      UInputBufferManager *InInputBufferManager) {
  Characters = InCharacters;
  InputBuffer = InInputBufferManager;
  CurrentFrame = 0;
}

void URollbackSimulationManager::InjectInput(uint8 PlayerID, uint16 InputMask) {
  InputBuffer->InjectInput(PlayerID, InputMask, CurrentFrame);
}

void URollbackSimulationManager::AdvanceSimulation() {
  const TArray<uint16> &Inputs = InputBuffer->GetInputsForFrame(CurrentFrame);
  for (int32 i = 0; i < Characters.Num(); i++)
    Characters[i]->SimulateFrame(FIXED_DELTA_TIME, Inputs[i]);

  SaveSnapshot(CurrentFrame);
  ++CurrentFrame;
}

void URollbackSimulationManager::SaveSnapshot(uint32 Frame) {
  FFrameSnapshot &Snapshot = FrameSnapshots.FindOrAdd(Frame);
  Snapshot.FrameNumber = Frame;

  if (Snapshot.SerializedStates.Num() < Characters.Num())
    Snapshot.SerializedStates.SetNum(Characters.Num());

  for (int8 i = 0; i < Characters.Num(); i++) {
    TArray<uint8> StateData;
    Characters[i]->SaveState(StateData);
    Snapshot.SerializedStates[i] = StateData;
  }
}

void URollbackSimulationManager::RollbackTo(uint32 Frame) {
  const FFrameSnapshot *Snapshot = FrameSnapshots.Find(Frame);

  if (!Snapshot) return;

  for (int32 i = 0; i < Characters.Num(); ++i)
    Characters[i]->LoadState(Snapshot->SerializedStates[i]);

  CurrentFrame = Frame;

  // Re-simulate from rollback point up to present
  // TODO fix that, the max frame is not the current
  const int32 MaxFrame = InputBuffer->GetMaxFrame();
  while (CurrentFrame < MaxFrame) AdvanceSimulation();
}
