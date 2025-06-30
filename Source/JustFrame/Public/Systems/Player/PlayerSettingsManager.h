// Every Frame is a Choice //
#pragma once

#include "CoreMinimal.h"
#include "Data/Structs/InputEnums.h"
#include "UObject/NoExportTypes.h"
#include "InputCoreTypes.h"
#include <SDL3/SDL.h>
#include "PlayerSettingsManager.generated.h"

USTRUCT()
struct FInputMapping {
  GENERATED_BODY()

  TMap<SDL_GamepadButton, uint16> SDLButtonToBitMask;
};

USTRUCT()
struct FPlayerControllerInfo {
  GENERATED_BODY()

  uint8 PlayerID;
  SDL_JoystickID JoystickID;
  bool bIsConnected;
};

USTRUCT()
struct FPlayerSettings {
  GENERATED_BODY()

  FInputMapping InputMap;
  FPlayerControllerInfo ControllerInfo;
};

UCLASS()
class JUSTFRAME_API UPlayerSettingsManager : public UObject {
  GENERATED_BODY()

public:
  void Init();

  const FInputMapping &GetInputMapping(uint8 PlayerID) const;
  const FInputMapping &GetMenuInputMapping(uint8 PlayerID) const;

  TOptional<uint8> GetPlayerIDForJoystick(SDL_JoystickID JoystickID) const;
  TOptional<SDL_JoystickID> GetJoystickIDForPlayer(uint8 PlayerID) const;
  TMap<SDL_JoystickID, uint8> GetJoystickToPlayer() const;

  FInputMapping GetDefaultInputMapping() const;
  int32 GetNumPlayers() const { return PlayerSettingsList.Num(); }
  TArray<uint8> GetActivePlayerIDs() const;

  bool OnControllerConnected(SDL_JoystickID JoystickID);
  bool OnControllerDisconnected(SDL_JoystickID JoystickID);

private:
  mutable FInputMapping DefaultInputMappingCache;
  TArray<FPlayerSettings> PlayerSettingsList;
  void ForTestOnly_Autosetup();
};
