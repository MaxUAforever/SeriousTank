#include "Widgets/Gameplay/PlayerUIWidget.h"

#include "GameFramework/Gameplay/ST_GameplayGameState.h"
#include "Components/TextBlock.h"
#include "TimerManager.h"

namespace
{
	FText ConvertTimeToText(const float Time)
	{
		int32 Minutes = FMath::TruncToInt(Time / 60);
		int32 Seconds = FMath::TruncToInt(Time) % 60;

		FString MinutesPrefix = Minutes < 10 ? "0" : "";
		FString MinutesStr = MinutesPrefix + FString::FromInt(Minutes);

		FString SecondsPrefix = Seconds < 10 ? "0" : "";
		FString SecondsStr = SecondsPrefix + FString::FromInt(Seconds);

		return FText::FromString(MinutesStr + ":" + SecondsStr);
	}
}

void UPlayerUIWidget::NativeConstruct()
{
	Super::NativeConstruct();

	check(ScoreBlock != nullptr);
	check(RemainingTimeBlock != nullptr);

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	TimeRefreshRate = 1.0f;

	FTimerDelegate TimeUpdateDelegate = FTimerDelegate::CreateUObject(this, &ThisClass::UpdateTime);
	World->GetTimerManager().SetTimer(TimeRefreshHandler, TimeUpdateDelegate, TimeRefreshRate, true);

	if (AST_GameplayGameState* GameState = World->GetGameState<AST_GameplayGameState>())
	{
		GameState->OnScoreHasChanged.BindUObject(this, &ThisClass::UpdateScore);
	}
}

void UPlayerUIWidget::UpdateScore(int32 NewScore)
{
	ScoreBlock->SetText(FText::FromString(FString::FromInt(NewScore)));
}

void UPlayerUIWidget::UpdateTime()
{
	if (UWorld* World = GetWorld())
	{
		if (AST_GameplayGameState* GameState = World->GetGameState<AST_GameplayGameState>())
		{
			RemainingTimeBlock->SetText(ConvertTimeToText(GameState->GetRemainingTime()));
		}
	}
}
