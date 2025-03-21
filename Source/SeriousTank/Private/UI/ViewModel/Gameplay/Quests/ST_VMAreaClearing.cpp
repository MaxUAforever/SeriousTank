#include "UI/ViewModel/Gameplay/Quests/ST_VMAreaClearing.h"

#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "Subsystems/QuestSubsystem/Tasks/ST_QuestTask_AIAreaClearing.h"
#include "QuestSubsystem/Public/QuestSubsystem.h"
#include "UI/Widgets/Gameplay/Quests/ST_AreaClearingTaskWidget.h"

void UST_VMAreaClearing::Initialize(UUserWidget* RelatedWidget)
{
	AreaClearingTaskWidget = Cast<UST_AreaClearingTaskWidget>(RelatedWidget);
	if (!IsValid(AreaClearingTaskWidget))
	{
		UE_LOG(LogTemp, Warning, TEXT("UST_VMTargetShooting::Initialize: Faield to get TargetShootingWidget!"));
		return;
	}

	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogTemp, Warning, TEXT("UST_VMTargetShooting::Initialize: Faield to get World!"));
		return;
	}

	UQuestSubsystem* QuestSubsystem = World->GetGameInstance()->GetSubsystem<UQuestSubsystem>();
	if (!IsValid(QuestSubsystem))
	{
		UE_LOG(LogTemp, Warning, TEXT("UST_VMTargetShooting::Initialize: QuestSubsystem is not valid!"));
		return;
	}

	AreaClearingTask = Cast<UST_QuestTask_AIAreaClearing>(QuestSubsystem->GetTrackedQuestTask());
	if (!IsValid(AreaClearingTask))
	{
		UE_LOG(LogTemp, Warning, TEXT("ViewModelTimeLimitedTask::Initialize: QuestSubsystem is not valid!"));
		return;
	}

	OnWaveChangedDelegateHandle = AreaClearingTask->OnAreaClearingWaveChangedDelegate.AddUObject(this, &ThisClass::OnWaveChanged);
	OnEnemyAmountChangedDelegate = AreaClearingTask->OnAreaClearingWaveEnemyAmountChangedDelegate.AddUObject(this, &ThisClass::OnEnemyAmountChanged);

	OnWaveChanged(AreaClearingTask->GetCurrentWaveIndex());
	OnEnemyAmountChanged(AreaClearingTask->GetCurrentWaveAIAmount());
}

void UST_VMAreaClearing::Deinitialize()
{
	if (IsValid(AreaClearingTask))
	{
		AreaClearingTask->OnAreaClearingWaveChangedDelegate.Remove(OnWaveChangedDelegateHandle);
		AreaClearingTask->OnAreaClearingWaveEnemyAmountChangedDelegate.Remove(OnEnemyAmountChangedDelegate);
	}
}

void UST_VMAreaClearing::OnWaveChanged(int32 InWaveIndex)
{
	if (IsValid(AreaClearingTaskWidget))
	{
		AreaClearingTaskWidget->SetWaveIndex(InWaveIndex);
	}
}

void UST_VMAreaClearing::OnEnemyAmountChanged(int32 InEnemyAmount)
{
	if (IsValid(AreaClearingTaskWidget))
	{
		AreaClearingTaskWidget->SetWaveEnemyAmount(InEnemyAmount);
	}
}