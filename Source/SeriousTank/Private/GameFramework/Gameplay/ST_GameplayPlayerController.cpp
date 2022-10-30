#include "GameFramework/Gameplay/ST_GameplayPlayerController.h"

#include "GameFramework/Gameplay/ST_GameplayGameState.h"
#include "Engine/World.h"

#include "GameFramework/Pawn.h"

void AST_GameplayPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputEnabled(false);
	if (UWorld* World = GetWorld())
	{
		if (AST_GameplayGameState* GameState = World->GetGameState<AST_GameplayGameState>())
		{
			GameState->OnPreStartCountdownEnded.BindUObject(this, &ThisClass::SetInputEnabled, true);
		}
	}
}

void AST_GameplayPlayerController::SetInputEnabled(bool IsEnabled)
{
	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn)
	{
		return;
	}

	if (IsEnabled)
	{
		ControlledPawn->EnableInput(this);
	}
	else
	{
		ControlledPawn->DisableInput(this);
	}
}
