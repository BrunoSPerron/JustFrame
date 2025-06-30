// Every Frame is a Choice //
#include "Systems/Player/PlayerSettingsManager.h"
#include <SDL3/SDL.h>

void UPlayerSettingsManager::Init() {
  PlayerSettingsList.Empty();

  for (uint8 PlayerID : GetActivePlayerIDs()) {
    FPlayerSettings Settings;
    Settings.InputMap = GetDefaultInputMapping();
    PlayerSettingsList.Add(Settings);
  }
}

FInputMapping UPlayerSettingsManager::GetDefaultInputMapping() const {
  FInputMapping Mapping;

  Mapping.SDLButtonToBitMask.Add(SDL_GAMEPAD_BUTTON_DPAD_UP, static_cast<uint16>(EInputBit::Up));
  Mapping.SDLButtonToBitMask.Add(SDL_GAMEPAD_BUTTON_DPAD_DOWN,
                                 static_cast<uint16>(EInputBit::Down));
  Mapping.SDLButtonToBitMask.Add(SDL_GAMEPAD_BUTTON_DPAD_LEFT,
                                 static_cast<uint16>(EInputBit::Left));
  Mapping.SDLButtonToBitMask.Add(SDL_GAMEPAD_BUTTON_DPAD_RIGHT,
                                 static_cast<uint16>(EInputBit::Right));

  Mapping.SDLButtonToBitMask.Add(SDL_GAMEPAD_BUTTON_SOUTH, static_cast<uint16>(EInputBit::Guard));
  Mapping.SDLButtonToBitMask.Add(SDL_GAMEPAD_BUTTON_EAST, static_cast<uint16>(EInputBit::Step));
  Mapping.SDLButtonToBitMask.Add(SDL_GAMEPAD_BUTTON_WEST, static_cast<uint16>(EInputBit::Punch));
  Mapping.SDLButtonToBitMask.Add(SDL_GAMEPAD_BUTTON_NORTH, static_cast<uint16>(EInputBit::Kick));

  Mapping.SDLButtonToBitMask.Add(SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER,
                                 static_cast<uint16>(EInputBit::Low));
  Mapping.SDLButtonToBitMask.Add(SDL_GAMEPAD_BUTTON_LEFT_SHOULDER,
                                 static_cast<uint16>(EInputBit::High));

  return Mapping;
}

TOptional<uint8> UPlayerSettingsManager::GetPlayerIDForJoystick(SDL_JoystickID JoystickID) const {
  for (int32 i = 0; i < PlayerSettingsList.Num(); ++i) {
    const FPlayerControllerInfo &Info = PlayerSettingsList[i].ControllerInfo;
    if (Info.bIsConnected && Info.JoystickID == JoystickID) {
      return static_cast<uint8>(i);
    }
  }
  return {};
}

TMap<SDL_JoystickID, uint8> UPlayerSettingsManager::GetJoystickToPlayer() const {
  TMap<SDL_JoystickID, uint8> Result;
  for (int32 i = 0; i < PlayerSettingsList.Num(); ++i) {
    const FPlayerControllerInfo &Info = PlayerSettingsList[i].ControllerInfo;
    if (Info.bIsConnected && Info.JoystickID != -1) {
      Result.Add(Info.JoystickID, static_cast<uint8>(i));
    }
  }
  return Result;
}

TOptional<SDL_JoystickID> UPlayerSettingsManager::GetJoystickIDForPlayer(uint8 PlayerID) const {
  if (PlayerSettingsList.IsValidIndex(PlayerID)) {
    const FPlayerSettings &Settings = PlayerSettingsList[PlayerID];
    if (Settings.ControllerInfo.bIsConnected) return Settings.ControllerInfo.JoystickID;
  }
  return {};
}

TArray<uint8> UPlayerSettingsManager::GetActivePlayerIDs() const {
  TArray<uint8> Result;
  for (int32 i = 0; i < PlayerSettingsList.Num(); ++i) {
    if (PlayerSettingsList[i].ControllerInfo.bIsConnected) Result.Add(static_cast<uint8>(i));
  }
  return Result;
}

bool UPlayerSettingsManager::OnControllerConnected(SDL_JoystickID JoystickID) {
  UE_LOG(LogTemp, Log, TEXT("OnControllerConnected called with JoystickID: %d"), JoystickID);
  // Check if this joystick is already mapped
  for (int32 i = 0; i < PlayerSettingsList.Num(); ++i) {
    if (PlayerSettingsList[i].ControllerInfo.bIsConnected &&
        PlayerSettingsList[i].ControllerInfo.JoystickID == JoystickID)
      return false;
  }
  // Try to reuse a disconnected player slot
  for (int32 i = 0; i < PlayerSettingsList.Num(); ++i) {
    FPlayerSettings &Settings = PlayerSettingsList[i];
    if (!Settings.ControllerInfo.bIsConnected) {
      Settings.ControllerInfo.JoystickID = JoystickID;
      Settings.ControllerInfo.bIsConnected = true;
      UE_LOG(LogTemp, Log,
             TEXT("UPlayerSettingsManager Joystick reconnected: Player %d, Stick: %d"), i,
             JoystickID);
      return true;
    }
  }
  // No free slot, create a new player
  FPlayerSettings Settings;
  Settings.InputMap = GetDefaultInputMapping();
  Settings.ControllerInfo.PlayerID = PlayerSettingsList.Num();
  Settings.ControllerInfo.JoystickID = JoystickID;
  Settings.ControllerInfo.bIsConnected = true;
  PlayerSettingsList.Add(Settings);
  UE_LOG(LogTemp, Log, TEXT("UPlayerSettingsManager Joystick connected: Player %d, Stick: %d"),
         PlayerSettingsList.Num() - 1, JoystickID);
  return true;
}

bool UPlayerSettingsManager::OnControllerDisconnected(SDL_JoystickID JoystickID) {
  for (int32 i = 0; i < PlayerSettingsList.Num(); ++i) {
    FPlayerSettings &Settings = PlayerSettingsList[i];
    if (Settings.ControllerInfo.bIsConnected && Settings.ControllerInfo.JoystickID == JoystickID) {
      Settings.ControllerInfo.bIsConnected = false;
      Settings.ControllerInfo.JoystickID = -1;
      UE_LOG(LogTemp, Warning,
             TEXT("UPlayerSettingsManager Joystick disconnected: Player %d, Stick: %d"), i,
             JoystickID);
      return true;
    }
  }
  return false;
}

const FInputMapping &UPlayerSettingsManager::GetInputMapping(uint8 PlayerID) const {
  if (PlayerSettingsList.IsValidIndex(PlayerID)) {
    return PlayerSettingsList[PlayerID].InputMap;
  }
  DefaultInputMappingCache = GetDefaultInputMapping();
  return DefaultInputMappingCache;
}

const FInputMapping &UPlayerSettingsManager::GetMenuInputMapping(uint8 PlayerID) const {
  return GetInputMapping(PlayerID);
}
