#include "UI/Widgets/Gameplay/PlayerUIWidget.h"

#include "Components/AudioComponent.h"
#include "Components/TextBlock.h"
#include "GameFramework/Gameplay/TargetShooting/ST_TargetShootingGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "TimerManager.h"
#include "UI/Utils/ST_WidgetUtilsLibrary.h"
#include "UI/ViewModel/Gameplay/ST_VMHealth.h"
#include "UI/Widgets/Gameplay/ST_HealthUserWidget.h"

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

	TimeRefreshRate = 0.5f;

	FTimerDelegate TimeUpdateDelegate = FTimerDelegate::CreateUObject(this, &ThisClass::UpdateTime);
	World->GetTimerManager().SetTimer(TimeRefreshHandler, TimeUpdateDelegate, TimeRefreshRate, true);

	if (AST_TargetShootingGameState* GameState = World->GetGameState<AST_TargetShootingGameState>())
	{
		GameState->OnScoreHasChanged.BindUObject(this, &ThisClass::UpdateScore);
		GameState->OnPreStartCountdownChanged.BindUObject(this, &ThisClass::UpdatePreStartTime);
	}

	HealthViewModel = NewObject<UST_VMHealth>(GetOuter(), UST_VMHealth::StaticClass());
	HealthViewModel->Initialize(UGameplayStatics::GetPlayerController(GetOuter(), 0));
	HealthViewModel->OnHealthChangedDelegate.BindUObject(HealthUserWidget, &UST_HealthUserWidget::SetHealthValue);
	HealthViewModel->OnMaxHealthChangedDelegate.BindUObject(HealthUserWidget, &UST_HealthUserWidget::SetMaxHealthValue);

	UISoundsComponent = UGameplayStatics::SpawnSound2D(World, CountdownSound); 
}

void UPlayerUIWidget::UpdateScore(int32 NewScore)
{
	ScoreBlock->SetText(FText::FromString(FString::FromInt(NewScore)));
	
	// Immideately update time block value after changing.
	UpdateTime();
}

void UPlayerUIWidget::UpdateTime()
{
	if (UWorld* World = GetWorld())
	{
		if (AST_TargetShootingGameState* GameState = World->GetGameState<AST_TargetShootingGameState>())
		{
			RemainingTimeBlock->SetText(UST_WidgetUtilsLibrary::ConvertTimeToText(GameState->GetRemainingTime()));
		}
	}
}

void UPlayerUIWidget::UpdatePreStartTime(int32 NewTime)
{
	FText PreStartTimeBlockValue = NewTime > 0 ? FText::FromString(FString::FromInt(NewTime)) : FText::FromString(TEXT("Start!"));
	PreStartTimeBlock->SetText(PreStartTimeBlockValue);
	
	if (NewTime <= 0)
	{
		if (UWorld* World = GetWorld())
		{
			FTimerHandle TextBlockVisibilityTimerHandle;
			FTimerDelegate TextBlockVisibilityDelegate = FTimerDelegate::CreateUObject(this, &ThisClass::HidePreStartTimeBlock);
			World->GetTimerManager().SetTimer(TextBlockVisibilityTimerHandle, TextBlockVisibilityDelegate, 1.5f, false);
		}

		if (UISoundsComponent)
		{
			UISoundsComponent->SetSound(StartGameSound);
		}
	}
}

void UPlayerUIWidget::HidePreStartTimeBlock()
{
	PreStartTimeBlock->SetVisibility(ESlateVisibility::Hidden);
}
