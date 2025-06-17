// Every Frame is a Choice //
#include "Editor/SDLDaemonEditorSubsystem.h"
#include "Editor.h"
#include "HAL/PlatformProcess.h"
#include "Misc/Paths.h"

void USDLDaemonEditorSubsystem::Initialize(FSubsystemCollectionBase &Collection) {
  FEditorDelegates::BeginPIE.AddUObject(this, &USDLDaemonEditorSubsystem::OnBeginPIE);
  FEditorDelegates::EndPIE.AddUObject(this, &USDLDaemonEditorSubsystem::OnEndPIE);
}

void USDLDaemonEditorSubsystem::Deinitialize() {
  FEditorDelegates::BeginPIE.RemoveAll(this);
  FEditorDelegates::EndPIE.RemoveAll(this);
}

void USDLDaemonEditorSubsystem::OnBeginPIE(bool bIsSimulating) {
  if (DaemonProcHandle.IsValid()) return; // Already running

  const FString ProjectDir = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir());
  const FString DaemonPath = FPaths::Combine(ProjectDir, TEXT("src/SDLInputDaemon"));
  DaemonProcHandle = FPlatformProcess::CreateProc(*DaemonPath, nullptr, true, false, false, nullptr,
                                                  0, nullptr, nullptr);
}

void USDLDaemonEditorSubsystem::OnEndPIE(bool bIsSimulating) {
  if (DaemonProcHandle.IsValid()) {
    FPlatformProcess::TerminateProc(DaemonProcHandle);
    FPlatformProcess::CloseProc(DaemonProcHandle);
    DaemonProcHandle.Reset();
  }
}
