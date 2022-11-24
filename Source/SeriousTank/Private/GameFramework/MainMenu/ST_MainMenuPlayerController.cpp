#include "GameFramework/MainMenu/ST_MainMenuPlayerController.h"

#include "GameFramework/MainMenu/ST_MainMenuGameState.h"
#include "GameFramework/ST_GameInstance.h"

#include "Engine/World.h"

void AST_MainMenuPlayerController::OnNewGameStarted()
{
	const UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	AST_MainMenuGameState* GameState = World->GetGameState<AST_MainMenuGameState>();
	if (!GameState)
	{
		return;
	}

	if (UST_GameInstance* GameInstance = World->GetGameInstance<UST_GameInstance>())
	{
		if (const FGameplayLevelInfo* LevelInfo = GameInstance->GetLevelInfo(GameState->GetCurrentGameLevelIndex()))
		{
            ClientTravel(LevelInfo->LevelName, ETravelType::TRAVEL_Relative, true);
		}
	}
}

void AST_MainMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();
    
    SetShowMouseCursor(true);
	SetInputMode(FInputModeGameAndUI{});
}
