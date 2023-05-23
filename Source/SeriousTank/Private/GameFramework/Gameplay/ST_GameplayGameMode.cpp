#include "GameFramework/Gameplay/ST_GameplayGameMode.h"

#include "Actors/Pawns/ST_BaseTrackedVehicle.h"
#include "GameFramework/Gameplay/ST_GameplayGameState.h"
#include "GameFramework/Gameplay/ST_GameplayPlayerState.h"
#include "Subsystems/ObjectSpawnSubsystem/ObjectSpawnSubsystem.h"

#include "Engine/World.h"
#include "TimerManager.h"

AST_GameplayGameMode::AST_GameplayGameMode()
{
    bUseSeamlessTravel = true;
}

void AST_GameplayGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (UObjectSpawnSubsystem* ObjectSpawnSubsystem = GetWorld()->GetSubsystem<UObjectSpawnSubsystem>())
	{
		for (FObjectTypeSpawnParams TypeSpawnParams : ObjectsSpawnParameters)
		{
			ObjectSpawnSubsystem->AddObjectSpawnManager(TypeSpawnParams.SpawnObjectType, TypeSpawnParams.SpawnParameters);
		}
	}
}

UClass* AST_GameplayGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
    if (UWorld* World = GetWorld())
    {
        if (APlayerController* PC = World->GetFirstPlayerController())
        {
            if (AST_GameplayPlayerState* PlayerState = PC->GetPlayerState<AST_GameplayPlayerState>())
            {
				if (TSubclassOf<AST_BaseVehicle> VehicleClass = PlayerState->GetVehicleInfo().VehicleClass)
				{
					return VehicleClass;
				}
            }
        }
    }
    
    return Super::GetDefaultPawnClassForController_Implementation(InController);
}

bool AST_GameplayGameMode::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
	bool bResult = Super::SetPause(PC, CanUnpauseDelegate);
	if (bResult)
	{
		OnGamePaused.ExecuteIfBound(true);
	}

	return bResult;
}

bool AST_GameplayGameMode::ClearPause()
{
	bool bResult = Super::ClearPause();
	if (bResult)
	{
		OnGamePaused.ExecuteIfBound(false);
	}

	return bResult;
}

void AST_GameplayGameMode::InitGameState()
{
	Super::InitGameState();

	if (AST_GameplayGameState* GameplayGameState = Cast<AST_GameplayGameState>(GameState))
	{
		GameplayGameState->SetPreStartCountdownTime(BaseGameData.PreStartCountdownTime);
	}
}
