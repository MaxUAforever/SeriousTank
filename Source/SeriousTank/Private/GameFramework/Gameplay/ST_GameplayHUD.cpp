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

void AST_GameplayHUD::SwitchToPauseWidget()
{
    SwitchToWidget(PauseWidget);
}

void AST_GameplayHUD::SwitchToFinalScoreWidget()
{
    SwitchToWidget(FinalScoreWidget);
}

void AST_GameplayHUD::SwitchFromSettingsWidget()
{
    SwitchToPauseWidget();
}

void AST_GameplayHUD::OnGameTimeEnded()
{
    SwitchToFinalScoreWidget();
}

void AST_GameplayHUD::OnGameIsPaused(bool IsPaused)
{
	if (PauseWidget)
	{
		PauseWidget->SetVisibility(IsPaused ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}
