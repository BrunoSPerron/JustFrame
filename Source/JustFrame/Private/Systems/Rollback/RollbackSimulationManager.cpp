// Every Frame is a Choice //
#include "Systems/Rollback/RollbackSimulationManager.h"
#include "Characters/RollbackCharacter.h"
#include "Data/Consts.h"
#include "Data/LogCategories.h"
#include "Kismet/GameplayStatics.h"

void URollbackSimulationManager::Init(TArray<ARollbackCharacter *> InCharacters,
                                      UInputBufferManager *InInputBufferManager,
                                      TMap<uint8, int32> InPlayerToCharacter) {
  Characters = InCharacters;
  InputBuffer = InInputBufferManager;
  PlayerToCharacter = InPlayerToCharacter;
  SaveSnapshot(0);
  CurrentFrame = 1;
}

void URollbackSimulationManager::InjectInput(uint8 PlayerID, uint16 InputMask) {
  int32 CharacterIndex = PlayerToCharacter.Contains(PlayerID) ? PlayerToCharacter[PlayerID] : -1;
  if (CharacterIndex >= 0 && CharacterIndex < Characters.Num()) {
    InputBuffer->InjectInput(CharacterIndex, InputMask, CurrentFrame);
  }
}

void URollbackSimulationManager::AdvanceSimulation() {
  const TArray<uint16> &Inputs = InputBuffer->GetInputsForFrame(CurrentFrame);
  for (const TPair<uint8, int32> &Pair : PlayerToCharacter) {
    uint8 PlayerID = Pair.Key;
    int32 CharacterIndex = Pair.Value;
    if (CharacterIndex >= 0 && CharacterIndex < Characters.Num() && PlayerID < Inputs.Num())
      Characters[CharacterIndex]->SimulateFrame(FIXED_DELTA_TIME, Inputs[PlayerID]);
  }
  SaveSnapshot(CurrentFrame);
  ++CurrentFrame;
}

const FCharacterState
URollbackSimulationManager::GetCharacterStateAtFrame(int32 Frame, uint8 CharacterIndex) const {
  const FFrameSnapshot *Snapshot = FrameSnapshots.Find(Frame);
  checkf(
      Snapshot,
      TEXT("URollbackSimulationManager::GetCharacterStateAtFrame No snapshot found for frame %d"),
      Frame);

  checkf(Snapshot->SerializedStates.IsValidIndex(CharacterIndex),
         TEXT("URollbackSimulationManager::GetCharacterStateAtFrame Invalid character index %d at "
              "frame %d"),
         CharacterIndex, Frame);

  const TArray<uint8> &Data = Snapshot->SerializedStates[CharacterIndex];
  checkf(Data.Num() == sizeof(FCharacterState),
         TEXT("URollbackSimulationManager::GetCharacterStateAtFrame Serialized state size mismatch "
              "(%d bytes) at frame %d, index %d"),
         Data.Num(), Frame, CharacterIndex);

  FCharacterState State;
  FMemory::Memcpy(&State, Data.GetData(), sizeof(FCharacterState));
  return State;
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
  const int32 MaxFrame = InputBuffer->GetMaxFrame();
  while (CurrentFrame < MaxFrame) AdvanceSimulation();
}
