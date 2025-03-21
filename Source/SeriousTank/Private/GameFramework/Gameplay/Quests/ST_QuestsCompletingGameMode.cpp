#include "GameFramework/Gameplay/Quests/ST_QuestsCompletingGameMode.h"

#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "QuestSubsystem/Public/QuestSubsystem.h"
#include "QuestSubsystem/Public/Components/QuestProvider.h"

AST_QuestsCompletingGameMode::AST_QuestsCompletingGameMode()
{
	QuestProvider = CreateDefaultSubobject<UQuestProvider>(TEXT("QuestProvider"));
}

void AST_QuestsCompletingGameMode::OnPreStartCountdownEneded()
{
	StartQuests();
}

void AST_QuestsCompletingGameMode::StartQuests()
{
	UGameInstance* GameInstance = GetGameInstance();
	if (!IsValid(GameInstance))
	{
		UE_LOG(LogTemp, Error, TEXT("AST_TargetShootingGameMode::BeginPlay: GameInstance is not valid"));
		return;
	}

	UQuestSubsystem* QuestSubsystem = GetGameInstance()->GetSubsystem<UQuestSubsystem>();
	if (!IsValid(QuestSubsystem))
	{
		UE_LOG(LogTemp, Warning, TEXT("AST_TargetShootingGameMode::BeginPlay: QuestSubsystem is not valid"));
		return;
	}

	for (const FQuestID& QuestID : QuestProvider->GetQuestIDs())
	{
		bool bSuccess = QuestSubsystem->StartQuest(QuestID);
		if (!bSuccess)
		{
			QuestSubsystem->GetOnQuestsInitializedDelegate().AddUObject(this, &ThisClass::StartQuests);
			return;
		}
	}

	UBaseQuestTask* TackedQuestTask = QuestSubsystem->GetTrackedQuestTask();
	if (IsValid(TackedQuestTask))
	{
		TackedQuestTask->OnQuestTaskCompletedDelegate.AddUObject(this, &ThisClass::OnQuestTaskCompleted);
	}
}

void AST_QuestsCompletingGameMode::OnQuestTaskCompleted(FTaskID TaskID, EQuestTaskCompleteResult TaskCompleteResult)
{
	TriggerGameFinish();
}
