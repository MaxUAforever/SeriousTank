#include "GameFramework/Gameplay/ST_GameplayHUD.h"

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
}

void AST_GameplayHUD::OnGameTimeEnded()
{
	APlayerController* Controller = Cast<APlayerController>(GetOwner());
	FinalScoreWidget = CreateWidget<UUserWidget>(Controller, FinalScoreWidgetClass);
	if (FinalScoreWidget)
	{
		GameUIWidget->SetVisibility(ESlateVisibility::Collapsed);

		FinalScoreWidget->AddToViewport();
	}
}
