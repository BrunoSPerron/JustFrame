#include "Characters/MoveInstance.h"

bool FMoveInstance::IsStartup() const { return Move && CurrentFrame < Move->StartupFrames; }

bool FMoveInstance::IsActive() const {
  return Move && CurrentFrame >= Move->StartupFrames &&
         CurrentFrame < (Move->StartupFrames + Move->ActiveFrames);
}

bool FMoveInstance::IsRecovery() const {
  return Move && CurrentFrame >= (Move->StartupFrames + Move->ActiveFrames);
}

bool FMoveInstance::IsDone() const {
  if (!Move) return true;

  const int32 RecoveryFrames = bMoveConnected ? Move->HitRecoveryFrames : Move->WhiffRecoveryFrames;
  return CurrentFrame >= (Move->StartupFrames + Move->ActiveFrames + RecoveryFrames);
}

void FMoveInstance::Tick() {
  if (Move) ++CurrentFrame;
}
