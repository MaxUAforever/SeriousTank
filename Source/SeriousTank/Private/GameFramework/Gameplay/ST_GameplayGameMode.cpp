#include "GameFramework/Gameplay/ST_GameplayGameMode.h"

#include "Actors/GameplayActors/ST_ShootTarget.h"
#include "Actors/Pawns/ST_BaseTrackedVehicle.h"
#include "GameFramework/Gameplay/ST_GameplayGameState.h"
#include "GameFramework/Gameplay/Utils/ST_TargetRespawnManager.h"
#include "GameFramework/ST_GameInstance.h"

#include "TimerManager.h"

UClass* AST_GameplayGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	UST_GameInstance* GameInstance = GetGameInstance<UST_GameInstance>();
	if (GameInstance)
	{
		return GameInstance->GetVehicleClass();
	}
	else
	{
		return Super::GetDefaultPawnClassForController_Implementation(InController);
	}
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
		GameplayGameState->OnTimeHasEnded.BindUObject(this, &ThisClass::OnGameOver);

		GameplayGameState->SetRemainingTime(GameData.StartTime);
		GameplayGameState->SetPreStartCountdownTime(GameData.PreStartCountdownTime);
	}
}

void AST_GameplayGameMode::OnTargetDestroyed(AActor* DestroyedActor)
{
	if (AST_GameplayGameState* GameplayGameState = Cast<AST_GameplayGameState>(GameState))
	{
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
