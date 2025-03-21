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

	if (AST_GameplayGameState* GameState = World->GetGameState<AST_GameplayGameState>())
	{
		GameState->OnPreStartCountdownChanged.BindUObject(this, &ThisClass::UpdatePreStartTime);
	}

	UISoundsComponent = UGameplayStatics::SpawnSound2D(World, CountdownSound); 
}

void UST_PreStartCountdownWidget::UpdatePreStartTime(int32 NewTime)
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

void UST_PreStartCountdownWidget::HidePreStartTimeBlock()
{
	PreStartTimeBlock->SetVisibility(ESlateVisibility::Hidden);
}
