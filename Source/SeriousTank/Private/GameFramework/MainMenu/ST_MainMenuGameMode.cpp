#include "GameFramework/MainMenu/ST_MainMenuGameMode.h"

#include "Actors/Pawns/ST_BaseVehicle.h"
#include "GameFramework/MainMenu/Utils/ST_VehicleSpawnManager.h"
#include "GameFramework/ST_GameInstance.h"

void AST_MainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();

    VehicleSpawnManager = NewObject<UST_VehicleSpawnManager>(this);
    
    if (UST_GameInstance* GameInstance = GetGameInstance<UST_GameInstance>())
    {
        GameInstance->OnVehicleChanged.AddUObject(this, &ThisClass::SpawnVehicle);
        SpawnVehicle(GameInstance->GetVehicleClass());
    }
}

void AST_MainMenuGameMode::SpawnVehicle(TSubclassOf<AST_BaseVehicle> VehicleClass)
{
	VehicleSpawnManager->SpawnVehicle(VehicleClass);
}
