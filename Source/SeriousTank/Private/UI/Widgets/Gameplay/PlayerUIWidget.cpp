#include "UI/Widgets/Gameplay/PlayerUIWidget.h"

#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "UI/ViewModel/Gameplay/ST_VMHealth.h"
#include "UI/ViewModel/Gameplay/Quests/ST_VMActiveQuestTask.h"
#include "UI/Widgets/Gameplay/ST_HealthUserWidget.h"
#include "UI/Widgets/Gameplay/ST_PreStartCountdownWidget.h"

void UPlayerUIWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HealthViewModel = NewObject<UST_VMHealth>(GetOuter(), UST_VMHealth::StaticClass());
	HealthViewModel->Initialize();
	HealthViewModel->OnHealthChangedDelegate.BindUObject(HealthUserWidget, &UST_HealthUserWidget::SetHealthValue);
	HealthViewModel->OnMaxHealthChangedDelegate.BindUObject(HealthUserWidget, &UST_HealthUserWidget::SetMaxHealthValue);

	ActiveQuestTaskViewModel = NewObject<UST_VMActiveQuestTask>(GetOuter(), UST_VMActiveQuestTask::StaticClass());
	ActiveQuestTaskViewModel->OnTrackedTaskWidgetChangedDelegate.BindUObject(this, &ThisClass::OnTrackedTaskWidgetChanged);
	ActiveQuestTaskViewModel->Initialize();
}

void UPlayerUIWidget::OnTrackedTaskWidgetChanged(UUserWidget* InTrackedTaskWidget)
{
	if (TrackedQuestTaskWidget == InTrackedTaskWidget)
	{
		return;
	}

	RootCanvasPanel->RemoveChild(TrackedQuestTaskWidget);
	
	TrackedQuestTaskWidget = InTrackedTaskWidget;
	if (IsValid(TrackedQuestTaskWidget))
	{
		UCanvasPanelSlot* PanelSlot = RootCanvasPanel->AddChildToCanvas(TrackedQuestTaskWidget);

		PanelSlot->SetAnchors(FAnchors(0.1f, 0.1f, 0.9f, 0.8f));
		PanelSlot->SetOffsets(FMargin(0.f, 0.f));
	}
}
