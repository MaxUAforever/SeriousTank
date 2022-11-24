#include "GameFramework/Gameplay/ST_GameplayGameMode.h"

#include "Actors/GameplayActors/ST_ShootTarget.h"
#include "Actors/Pawns/ST_BaseTrackedVehicle.h"
#include "GameFramework/Gameplay/ST_GameplayGameState.h"
#include "GameFramework/Gameplay/ST_GameplayPlayerState.h"
#include "GameFramework/Gameplay/Utils/ST_TargetRespawnManager.h"

#include "Engine/World.h"
#include "TimerManager.h"

AST_GameplayGameMode::AST_GameplayGameMode()
{
    bUseSeamlessTravel = true;
}

UClass* AST_GameplayGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
    if (UWorld* World = GetWorld())
    {
        if (APlayerController* PC = World->GetFirstPlayerController())
        {
            if (AST_GameplayPlayerState* PlayerState = PC->GetPlayerState<AST_GameplayPlayerState>())
            {
                return PlayerState->GetVehicleInfo().VehicleClass;
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

void AST_GameplayGameMode::BeginPlay()
{
	Super::BeginPlay();

	TargetRespawnManager = NewObject<UST_TargetRespawnManager>(this);
	SpawnTarget();
}

void AST_GameplayGameMode::InitGameState()
{
	Super::InitGameState();

	if (AST_GameplayGameState* GameplayGameState = Cast<AST_GameplayGameState>(GameState))
	{
		GameplayGameState->SetRemainingTime(GameData.StartTime);
		GameplayGameState->SetPreStartCountdownTime(GameData.PreStartCountdownTime);
	}
}

void AST_GameplayGameMode::OnTargetDestroyed(AActor* DestroyedActor)
{
	if (AST_GameplayGameState* GameplayGameState = Cast<AST_GameplayGameState>(GameState))
	{
		GameplayGameState->AddScore(GameData.AddedScoreForTarget);
		GameplayGameState->AddRemainingTime(GameData.AddedTimeForTarget);
	}

	SpawnTarget();
}

void AST_GameplayGameMode::SpawnTarget()
{
	if (TargetRespawnManager)
	{
		AST_ShootTarget* Target = TargetRespawnManager->SpawnTarget(GameData.ShootingTargetClass);
		Target->OnDestroyed.AddDynamic(this, &ThisClass::OnTargetDestroyed);
	}
}
