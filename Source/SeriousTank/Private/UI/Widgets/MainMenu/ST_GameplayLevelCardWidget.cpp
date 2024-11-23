#include "UI/Widgets/MainMenu/ST_GameplayLevelCardWidget.h"

#include "GameFramework/MainMenu/ST_MainMenuGameState.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void UST_GameplayLevelCardWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ChooseLevelButton->OnClicked.AddDynamic(this, &ThisClass::OnChooseLevelButtonClicked);

	UWorld* World = GetWorld();
	if (World)
	{
		if (AST_MainMenuGameState* GameState = World->GetGameState<AST_MainMenuGameState>())
		{
			OnLevelButtonClicked.AddUObject(GameState, &AST_MainMenuGameState::SetCurrentGameLevelIndex);
		}
	}
}

void UST_GameplayLevelCardWidget::SetLevelName(FText LevelName)
{
	LevelNameBlock->SetText(LevelName);
}

void UST_GameplayLevelCardWidget::SetLevelIndex(int32 NewLevelIndex)
{
	LevelIndex = NewLevelIndex;
}

void UST_GameplayLevelCardWidget::OnChooseLevelButtonClicked()
{
	OnLevelButtonClicked.Broadcast(LevelIndex);
}
