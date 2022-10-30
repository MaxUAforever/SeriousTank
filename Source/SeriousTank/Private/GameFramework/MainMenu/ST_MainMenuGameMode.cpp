#include "GameFramework/MainMenu/ST_MainMenuGameMode.h"

#include "GameFramework/MainMenu/Utils/ST_VehicleSpawnManager.h"
#include "GameFramework/ST_GameInstance.h"

void AST_MainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();

	VehicleSpawnManager = NewObject<UST_VehicleSpawnManager>(this);
	SpawnVehicle();
}

void AST_MainMenuGameMode::SpawnVehicle()
{
	if (UST_GameInstance* GameInstance = GetGameInstance<UST_GameInstance>())
	{
		VehicleSpawnManager->SpawnVehicle(GameInstance->GetVehicleClass());
	}
}
