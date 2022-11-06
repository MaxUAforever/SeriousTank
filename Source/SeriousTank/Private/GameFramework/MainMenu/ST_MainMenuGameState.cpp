#include "GameFramework/MainMenu/ST_MainMenuGameState.h"

#include "Engine/World.h"

#include "GameFramework/ST_GameInstance.h"

AST_MainMenuGameState::AST_MainMenuGameState()
{
	CurrentGameLevelIndex = 0;
}

void AST_MainMenuGameState::SetCurrentGameLevelIndex(int32 Index)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	if (UST_GameInstance* GameInstance = World->GetGameInstance<UST_GameInstance>())
	{
		const FGameplayLevelInfo* LevelInfo = GameInstance->GetLevelInfo(Index);
		if (LevelInfo)
		{
			CurrentGameLevelIndex = Index;
		}
	}
}
