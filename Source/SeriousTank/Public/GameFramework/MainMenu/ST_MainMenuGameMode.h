#pragma once

#include "GameFramework/GameModeBase.h"
#include "ST_MainMenuGameMode.generated.h"

class UST_VehicleSpawnManager;
struct FVehicleInfo;

UCLASS()
class SERIOUSTANK_API AST_MainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()
    
private:
	UST_VehicleSpawnManager* VehicleSpawnManager;

public:
    AST_MainMenuGameMode();
    
protected:
	virtual void BeginPlay() override;

private:
    UFUNCTION()
	void SpawnVehicle(FVehicleInfo VehicleClass);
};
