// Every Frame is a Choice //
#pragma once

#include "CoreMinimal.h"
#include "Game/GameManagerSubsystem.h"
#include "Systems/Player/PlayerSettingsManager.h"
#include "Systems/Input/InputBufferManager.h"
#include <set>
#include "UObject/Interface.h"
#include "IMenuCore.generated.h"

UINTERFACE(MinimalAPI)
class UMenuCore : public UInterface {
  GENERATED_BODY()
};

class IMenuCore {
  GENERATED_BODY()
public:
  virtual void ProcessInput(uint8 PlayerID, uint16 InputMask) = 0;

  void SetActivePlayers(const TSet<uint8> &Players) { ActivePlayers = Players; }
  const TSet<uint8> &GetActivePlayers() const { return ActivePlayers; }

private:
  TSet<uint8> ActivePlayers;
};