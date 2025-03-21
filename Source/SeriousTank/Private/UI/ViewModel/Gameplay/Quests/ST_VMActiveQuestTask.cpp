#include "UI/ViewModel/Gameplay/Quests/ST_VMActiveQuestTask.h"

#include "Engine/GameInstance.h"
#include "Blueprint/UserWidget.h"
#include "QuestSubsystem/Public/QuestSubsystem.h"
#include "QuestSubsystem/Public/UI/ViewModel/ViewModelBase.h"

void UST_VMActiveQuestTask::Initialize()
{
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogTemp, Error, TEXT("UST_VMActiveQuestTask::Initialize: World is not valid!"));
		return;
	}

	UGameInstance* GameInstance = World->GetGameInstance();
	if (!IsValid(GameInstance))
	{
		UE_LOG(LogTemp, Error, TEXT("UST_VMActiveQuestTask::Initialize: GameInstance is not valid!"));
		return;
	}

	UQuestSubsystem* QuestSubsystem = GameInstance->GetSubsystem<UQuestSubsystem>();
	if (!IsValid(QuestSubsystem))
	{
		UE_LOG(LogTemp, Warning, TEXT("UST_VMActiveQuestTask::Initialize: QuestSubsystem is not valid!"));
		return;
	}

	QuestSubsystem->GetOnTrackedTaskWidgetCreatedDelegate().AddUObject(this, &ThisClass::OnTrackedTaskWidgetChanged);
	OnTrackedTaskWidgetChanged(QuestSubsystem->GetTrackedTaskWidget(), nullptr);
}

void UST_VMActiveQuestTask::OnTrackedTaskWidgetChanged(UUserWidget* TaskWidget, UViewModelBase* TaskViewModel)
{
	OnTrackedTaskWidgetChangedDelegate.ExecuteIfBound(TaskWidget);
}
