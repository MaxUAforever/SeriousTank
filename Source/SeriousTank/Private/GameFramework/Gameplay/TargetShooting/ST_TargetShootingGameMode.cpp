#include "GameFramework/Gameplay/TargetShooting/ST_TargetShootingGameMode.h"

#include "Actors/GameplayActors/ST_ShootTarget.h"
#include "Engine/World.h"
#include "GameFramework/Gameplay/TargetShooting/ST_TargetShootingGameState.h"
#include "Subsystems/ObjectSpawnSubsystem/ObjectSpawnSubsystem.h"

DEFINE_LOG_CATEGORY_STATIC(TargetShootingGameMode, Display, All);

void AST_TargetShootingGameMode::BeginPlay()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(TargetShootingGameMode, Warning, TEXT("BeginPlay: Failed to get world"));
		return;
	}

	World->AddOnActorSpawnedHandler(FOnActorSpawned::FDelegate::CreateUObject(this, &ThisClass::OnTargetSpawned));
	
	Super::BeginPlay();
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
}

void AST_TargetShootingGameMode::OnTargetSpawned(AActor* InTargetActor)
{
	AST_ShootTarget* Target = Cast<AST_ShootTarget>(InTargetActor);
	if (!Target)
	{
		return;
	}

	Target->OnDestroyed.AddDynamic(this, &ThisClass::OnTargetDestroyed);
}
