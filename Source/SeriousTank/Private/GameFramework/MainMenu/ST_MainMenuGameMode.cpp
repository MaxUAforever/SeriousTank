#include "GameFramework/MainMenu/ST_MainMenuGameMode.h"

#include "Actors/Pawns/ST_BaseVehicle.h"
#include "Core/ST_CoreTypes.h"
#include "GameFramework/MainMenu/ST_MainMenuPlayerState.h"
#include "GameFramework/MainMenu/Utils/ST_VehicleSpawnManager.h"
#include "GameFramework/ST_GameInstance.h"

AST_MainMenuGameMode::AST_MainMenuGameMode()
{
    bUseSeamlessTravel = true;
}

void AST_MainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();
    
    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }
    
    VehicleSpawnManager = NewObject<UST_VehicleSpawnManager>(this);
    
    if (APlayerController* PlayerController = World->GetFirstPlayerController())
    {
        if (AST_MainMenuPlayerState* PlayerState = PlayerController->GetPlayerState<AST_MainMenuPlayerState>())
        {
            PlayerState->OnCurrentVehicleChanged.BindUObject(this, &ThisClass::SpawnVehicle);
            SpawnVehicle(PlayerState->GetCurrentVehicle());
        }
    }
}

void AST_MainMenuGameMode::SpawnVehicle(FVehicleInfo VehicleInfo)
{
	VehicleSpawnManager->SpawnVehicle(VehicleInfo);
}
