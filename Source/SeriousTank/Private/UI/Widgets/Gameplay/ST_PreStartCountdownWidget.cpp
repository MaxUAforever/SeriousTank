#include "UI/Widgets/Gameplay/ST_PreStartCountdownWidget.h"

#include "Components/AudioComponent.h"
#include "Components/TextBlock.h"
#include "GameFramework/Gameplay/ST_GameplayGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "TimerManager.h"
#include "UI/Utils/ST_WidgetUtilsLibrary.h"
#include "UI/ViewModel/Gameplay/ST_VMHealth.h"
#include "UI/Widgets/Gameplay/ST_HealthUserWidget.h"

void UST_PreStartCountdownWidget::NativeConstruct()
{
	Super::NativeConstruct();

	check(PreStartTimeBlock != nullptr);

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	AST_GameplayGameState* GameState = World->GetGameState<AST_GameplayGameState>();
	if (!IsValid(GameState))
	{
		return;
	}

	UISoundsComponent = UGameplayStatics::SpawnSound2D(World, CountdownSound);

	GameState->OnPreStartCountdownStartedDelegate.AddUObject(this, &ThisClass::OnPrePreGameCountdownStarted);

	if (GameState->GetInternalGameState() == EInternalGameState::WaitingToInitialize)
	{
		PreStartTimeBlock->SetText(FText::FromString("Waiting..."));
	}
	else if (GameState->GetInternalGameState() == EInternalGameState::PreGameCountdown)
	{
		OnPrePreGameCountdownStarted();
	} 
}

void UST_PreStartCountdownWidget::BeginDestroy()
{
	Super::BeginDestroy();

	UWorld* World = GetWorld();
	if (IsValid(World))
	{
		World->GetTimerManager().ClearTimer(CountdownRefreshTimer);
		return;
	}
}

void UST_PreStartCountdownWidget::OnPrePreGameCountdownStarted()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	PreStartTimeBlock->SetVisibility(ESlateVisibility::Visible);
	World->GetTimerManager().SetTimer(CountdownRefreshTimer, this, &ThisClass::UpdatePreStartTime, 0.25f, true);
}

void UST_PreStartCountdownWidget::UpdatePreStartTime()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	AST_GameplayGameState* GameState = World->GetGameState<AST_GameplayGameState>();
	if (!IsValid(GameState))
	{
		return;
	}

	if (GameState->GetInternalGameState() != EInternalGameState::PreGameCountdown)
	{
		World->GetTimerManager().ClearTimer(CountdownRefreshTimer);
		return;
	}

	int32 NewTime = FMath::CeilToInt32(GameState->GetPreStartCountdownTime());

	FText PreStartTimeBlockValue = NewTime > 0 ? FText::FromString(FString::FromInt(NewTime)) : FText::FromString(TEXT("Start!"));
	PreStartTimeBlock->SetText(PreStartTimeBlockValue);
	
	if (NewTime <= 0)
	{
		FTimerHandle TextBlockVisibilityTimerHandle;
		FTimerDelegate TextBlockVisibilityDelegate = FTimerDelegate::CreateUObject(this, &ThisClass::HidePreStartTimeBlock);
			
		World->GetTimerManager().ClearTimer(CountdownRefreshTimer);
		World->GetTimerManager().SetTimer(TextBlockVisibilityTimerHandle, TextBlockVisibilityDelegate, 1.5f, false);

		if (UISoundsComponent)
		{
			UISoundsComponent->SetSound(StartGameSound);
		}
	}
}

void UST_PreStartCountdownWidget::HidePreStartTimeBlock()
{
	PreStartTimeBlock->SetVisibility(ESlateVisibility::Hidden);
}
