// Every Frame is a Choice
#include "Systems/Player/PlayerSettingsManager.h"
#include <SDL3/SDL.h>

void UPlayerSettingsManager::Init(int32 NumPlayers) {
  PlayerSettingsMap.Empty();

  for (uint8 PlayerID = 0; PlayerID < NumPlayers; ++PlayerID) {
    FPlayerSettings Settings;
    Settings.InputMap = GetDefaultInputMapping();
    PlayerSettingsMap.Add(PlayerID, Settings);
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
  for (const TPair<uint8, FPlayerSettings> &Elem : PlayerSettingsMap) {
    const FPlayerControllerInfo &Info = Elem.Value.ControllerInfo;
    if (Info.bIsConnected && Info.JoystickID == JoystickID) {
      return Elem.Key;
    }
  }
  return {};
}

TMap<SDL_JoystickID, uint8> UPlayerSettingsManager::GetJoystickToPlayer() const {
  TMap<SDL_JoystickID, uint8> Result;
  for (const TPair<uint8, FPlayerSettings> &Elem : PlayerSettingsMap) {
    const FPlayerControllerInfo &Info = Elem.Value.ControllerInfo;
    if (Info.bIsConnected && Info.JoystickID != -1) {
      Result.Add(Info.JoystickID, Elem.Key);
    }
  }
  return Result;
}

bool UPlayerSettingsManager::OnControllerConnected(SDL_JoystickID JoystickID) {
  for (TPair<uint8, FPlayerSettings> &Elem : PlayerSettingsMap) {
    FPlayerSettings &Settings = Elem.Value;
    if (!Settings.ControllerInfo.bIsConnected) {
      Settings.ControllerInfo.PlayerID = Elem.Key;
      Settings.ControllerInfo.JoystickID = JoystickID;
      Settings.ControllerInfo.bIsConnected = true;
      UE_LOG(LogTemp, Warning,
             TEXT("UPlayerSettingsManager Joystick connected: Player %u, Stick: %u"), Elem.Key,
             JoystickID);
      return true;
    }
  }
  return false;
}

bool UPlayerSettingsManager::OnControllerDisconnected(SDL_JoystickID JoystickID) {
  for (TPair<uint8, FPlayerSettings> &Elem : PlayerSettingsMap) {
    FPlayerSettings &Settings = Elem.Value;
    if (Settings.ControllerInfo.bIsConnected && Settings.ControllerInfo.JoystickID == JoystickID) {
      Settings.ControllerInfo = {};
      UE_LOG(LogTemp, Warning,
             TEXT("UPlayerSettingsManager Joystick disconnected: Player %u, Stick: %u"), Elem.Key,
             JoystickID);
      return true;
    }
  }
  return false;
}

const FInputMapping &UPlayerSettingsManager::GetInputMapping(uint8 PlayerID) const {
  const FPlayerSettings *Settings = PlayerSettingsMap.Find(PlayerID);
  if (Settings) return Settings->InputMap;

  DefaultInputMappingCache = GetDefaultInputMapping();
  return DefaultInputMappingCache;
}
