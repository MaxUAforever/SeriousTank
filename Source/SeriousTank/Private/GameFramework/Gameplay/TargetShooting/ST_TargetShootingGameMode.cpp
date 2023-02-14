#include "GameFramework/Gameplay/TargetShooting/ST_TargetShootingGameMode.h"

#include "Actors/GameplayActors/ST_ShootTarget.h"
#include "GameFramework/Gameplay/TargetShooting/ST_TargetShootingGameState.h"
#include "GameFramework/Gameplay/Utils/ST_TargetRespawnManager.h"

DEFINE_LOG_CATEGORY_STATIC(TargetShootingGameMode, Display, All);

void AST_TargetShootingGameMode::BeginPlay()
{
	Super::BeginPlay();

	TargetRespawnManager = NewObject<UST_TargetRespawnManager>(this);
	TargetRespawnManager->SetSpawnVolumeClass(GameData.TargetSpawnVolumeClass);
	TargetRespawnManager->SetShootTargetClass(GameData.ShootingTargetClass);

	SpawnTarget();
}

void AST_TargetShootingGameMode::InitGameState()
{
	Super::InitGameState();

	if (AST_TargetShootingGameState* GameplayGameState = Cast<AST_TargetShootingGameState>(GameState))
	{
		GameplayGameState->SetRemainingTime(GameData.StartTime);
	}
}

void AST_TargetShootingGameMode::OnTargetDestroyed(AActor* DestroyedActor)
{
	if (AST_TargetShootingGameState* GameplayGameState = Cast<AST_TargetShootingGameState>(GameState))
	{
		GameplayGameState->AddScore(GameData.AddedScoreForTarget);
		GameplayGameState->AddRemainingTime(GameData.AddedTimeForTarget);
	}

	SpawnTarget();
}

void AST_TargetShootingGameMode::SpawnTarget()
{
	if (!TargetRespawnManager)
	{
		UE_LOG(TargetShootingGameMode, Warning, TEXT("Failed to get pointer to TargetRespawnManager"))
		return;
	}
	
	if (AST_ShootTarget* Target = TargetRespawnManager->SpawnTarget())
	{
		Target->OnDestroyed.AddDynamic(this, &ThisClass::OnTargetDestroyed);
	}
}