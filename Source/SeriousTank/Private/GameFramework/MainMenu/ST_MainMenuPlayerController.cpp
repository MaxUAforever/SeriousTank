#include "GameFramework/MainMenu/ST_MainMenuPlayerController.h"

#include "GameFramework/MainMenu/ST_MainMenuGameState.h"
#include "GameFramework/ST_GameInstance.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

AST_MainMenuPlayerController::AST_MainMenuPlayerController()
{
	SetShowMouseCursor(true);
}

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
			UGameplayStatics::OpenLevel(World, FName(*LevelInfo->LevelName));
		}
	}
}

void AST_MainMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeGameAndUI{});
}