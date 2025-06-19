// Every Frame is a Choice //
#pragma once

#include "CoreMinimal.h"
#include "EditorSubsystem.h"
#include "SDLDaemonEditorSubsystem.generated.h"

/*
  Runs an SDL input daemon when PIE starts.

  On startup, the game will try to connect to this daemon for input. If it fails,
  it falls back to the in-project SDL input class — unless running from the editor,
  in which case it keeps retrying until the daemon is reachable.

  This is primarily a Linux workaround to avoid a bug where the editor loses
  control of mouse input when SDL is active.
*/
UCLASS()
class JUSTFRAME_API USDLDaemonEditorSubsystem : public UEditorSubsystem {
  GENERATED_BODY()

public:
  virtual void Initialize(FSubsystemCollectionBase &Collection) override;
  virtual void Deinitialize() override;

private:
  void OnBeginPIE(bool bIsSimulating);
  void OnEndPIE(bool bIsSimulating);

  FProcHandle DaemonProcHandle;
};
