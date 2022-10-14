#include "GameFramework/Gameplay/ST_GameplayGameMode.h"

#include "Actors/GameplayActors/ST_ShootTarget.h"
#include "Actors/Pawns/ST_BaseTrackedVehicle.h"
#include "GameFramework/Gameplay/ST_GameplayGameState.h"
#include "GameFramework/Gameplay/Utils/ST_TargetRespawnManager.h"

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
	}
}

void AST_GameplayGameMode::OnTargetDestroyed(AActor* DestroyedActor)
{
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
