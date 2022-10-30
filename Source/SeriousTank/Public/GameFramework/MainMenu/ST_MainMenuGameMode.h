#pragma once

#include "GameFramework/GameModeBase.h"
#include "ST_MainMenuGameMode.generated.h"

class UST_VehicleSpawnManager;

UCLASS()
class SERIOUSTANK_API AST_MainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

private:
	UST_VehicleSpawnManager* VehicleSpawnManager;

protected:
	virtual void BeginPlay() override;

private:
	void SpawnVehicle();
};
