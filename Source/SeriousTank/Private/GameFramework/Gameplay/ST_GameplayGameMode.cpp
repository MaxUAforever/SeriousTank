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
		GameplayGameState->SetActorTickEnabled(true);
	}
}

void AST_GameplayGameMode::OnTargetDestroyed(AActor* DestroyedActor)
{
	if (AST_GameplayGameState* GameplayGameState = Cast<AST_GameplayGameState>(GameState))
	{
		GameplayGameState->OnTimeHasEnded.BindUObject(this, &ThisClass::OnGameOver);

		GameplayGameState->AddScore(GameData.AddedScoreForTarget);
		GameplayGameState->AddTime(GameData.AddedTimeForTarget);
	}

	SpawnTarget();
}

void AST_GameplayGameMode::OnGameOver()
{
	if (AST_GameplayGameState* GameplayGameState = Cast<AST_GameplayGameState>(GameState))
	{
		GameplayGameState->SetActorTickEnabled(false);
	}
}

void AST_GameplayGameMode::SpawnTarget()
{
	if (TargetRespawnManager)
	{
		AST_ShootTarget* Target = TargetRespawnManager->SpawnTarget(GameData.ShootingTargetClass);
		Target->OnDestroyed.AddDynamic(this, &ThisClass::OnTargetDestroyed);
	}
}