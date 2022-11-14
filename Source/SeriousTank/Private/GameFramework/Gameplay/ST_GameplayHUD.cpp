#include "GameFramework/Gameplay/ST_GameplayHUD.h"

#include "GameFramework/Gameplay/ST_GameplayGameMode.h"
#include "GameFramework/Gameplay/ST_GameplayGameState.h"

#include "Blueprint/UserWidget.h"
#include "Engine/World.h"

void AST_GameplayHUD::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	if (AST_GameplayGameState* GameState = World->GetGameState<AST_GameplayGameState>())
	{
		GameState->OnTimeHasEnded.AddUObject(this, &ThisClass::OnGameTimeEnded);
	}
	if (AST_GameplayGameMode* GameMode = World->GetAuthGameMode<AST_GameplayGameMode>())
	{
		GameMode->OnGamePaused.BindUObject(this, &ThisClass::OnGameIsPaused);
	}

	PauseWidget = CreateAndAddWidget(PauseWidgetClass);
	FinalScoreWidget = CreateAndAddWidget(FinalScoreWidgetClass);
}

void AST_GameplayHUD::SetSettingsWidgetVisible(bool bShowSettings)
{
	if (PauseWidget && SettingsWidget)
	{
		PauseWidget->SetVisibility(bShowSettings ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
		SettingsWidget->SetVisibility(bShowSettings ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void AST_GameplayHUD::OnGameTimeEnded()
{
	if (FinalScoreWidget)
	{
		GameUIWidget->SetVisibility(ESlateVisibility::Hidden);
		FinalScoreWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AST_GameplayHUD::OnGameIsPaused(bool IsPaused)
{
	if (PauseWidget)
	{
		PauseWidget->SetVisibility(IsPaused ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}
