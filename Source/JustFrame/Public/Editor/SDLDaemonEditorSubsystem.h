#pragma once

#include "CoreMinimal.h"
#include "EditorSubsystem.h"
#include "SDLDaemonEditorSubsystem.generated.h"

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
