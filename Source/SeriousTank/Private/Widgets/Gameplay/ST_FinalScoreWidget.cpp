#include "Widgets/Gameplay/ST_FinalScoreWidget.h"

#include "GameFramework/Gameplay/ST_GameplayGameState.h"
#include "GameFramework/ST_GameInstance.h"
#include "Widgets/Utils/ST_WidgetUtilsLibrary.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"

void UST_FinalScoreWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayAgainButton->OnClicked.AddDynamic(this, &ThisClass::OnPlayAgainButtonClicked);
	MenuButton->OnClicked.AddDynamic(this, &ThisClass::OnMenuButtonClicked);

	if (UWorld* World = GetWorld())
	{
		if (AST_GameplayGameState* GameState = World->GetGameState<AST_GameplayGameState>())
		{
			GameState->OnTimeHasEnded.AddUObject(this, &ThisClass::OnGameIsOver);
		}
	}
}

void UST_FinalScoreWidget::OnPlayAgainButtonClicked()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);
	if (PlayerController)
	{
		PlayerController->RestartLevel();
	}
}

void UST_FinalScoreWidget::OnMenuButtonClicked()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	if (UST_GameInstance* GameInstance = World->GetGameInstance<UST_GameInstance>())
	{
		if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0))
		{
			PlayerController->ClientTravel(GameInstance->GetMainMenuLevelName(), ETravelType::TRAVEL_Relative);
		}
	}
}

void UST_FinalScoreWidget::OnGameIsOver()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	if (AST_GameplayGameState* GameState = World->GetGameState<AST_GameplayGameState>())
	{
		ScoreValueBlock->SetText(FText::FromString(FString::FromInt(GameState->GetScore())));
		TimeValueBlock->SetText(UST_WidgetUtilsLibrary::ConvertTimeToText(GameState->GetTotalPlayTime()));
		TargetsValueBlock->SetText(FText::FromString(FString::FromInt(GameState->GetScore())));
	}
}