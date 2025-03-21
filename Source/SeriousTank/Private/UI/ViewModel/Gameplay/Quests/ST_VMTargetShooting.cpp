#include "UI/ViewModel/Gameplay/Quests/ST_VMTargetShooting.h"

#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "Subsystems/QuestSubsystem/Tasks/ST_QuestTask_TargetShooting.h"
#include "QuestSubsystem/Public/QuestSubsystem.h"
#include "UI/Widgets/Gameplay/Quests/ST_TargetShootingWidget.h"

void UST_VMTargetShooting::Initialize(UUserWidget* RelatedWidget)
{
	Super::Initialize(RelatedWidget);

	TargetShootingWidget = Cast<UST_TargetShootingWidget>(RelatedWidget);
	if (!IsValid(TargetShootingWidget))
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

	TargetShootingTask = Cast<UST_QuestTask_TargetShooting>(QuestSubsystem->GetTrackedQuestTask());
	if (!IsValid(TargetShootingTask))
	{
		UE_LOG(LogTemp, Warning, TEXT("ViewModelTimeLimitedTask::Initialize: QuestSubsystem is not valid!"));
		return;
	}

	ScoreChangedDelegateHandle = TargetShootingTask->OnScoreChangedDelegate.AddUObject(this, &ThisClass::OnScoreChanged);

	OnScoreChanged(TargetShootingTask->GetScore());
}

void UST_VMTargetShooting::Deinitialize()
{
	if (IsValid(TargetShootingTask))
	{
		TargetShootingTask->OnScoreChangedDelegate.Remove(ScoreChangedDelegateHandle);
	}
}

void UST_VMTargetShooting::OnScoreChanged(int32 InScore)
{
	if (TargetShootingWidget)
	{
		TargetShootingWidget->SetScore(InScore);
	}
}