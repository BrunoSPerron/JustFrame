// Every Frame is a Choice
#include "Controllers/Player/FightPlayerController.h"

// unused
void AFightPlayerController::BeginPlay() {
  Super::BeginPlay();

  bAutoManageActiveCameraTarget = false;
  //SetIgnoreMoveInput(true); // Disables WASD, stick input, etc.
  //SetIgnoreLookInput(true); // Disables mouse/analog look input
  //DisableInput(this);       // Kills input stack for this controller
}
