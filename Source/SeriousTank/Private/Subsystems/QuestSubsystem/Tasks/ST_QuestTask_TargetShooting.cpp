#include "Subsystems/QuestSubsystem/Tasks/ST_QuestTask_TargetShooting.h"

#include "Actors/GameplayActors/ST_ShootTarget.h"
#include "Engine/World.h"
#include "Subsystems/ObjectSpawnSubsystem/ObjectSpawnManager.h"
#include "Subsystems/ObjectSpawnSubsystem/ObjectSpawnSubsystem.h"
#include "Subsystems/QuestSubsystem/Tasks/Data/ST_TargetShootingTaskInfoDataAsset.h"

void UST_QuestTask_TargetShooting::FillTaskInfo(const UQuestTaskInfoDataAsset* QuestInfoDataAsset)
{
	Super::FillTaskInfo(QuestInfoDataAsset);

	const UST_TargetShootingTaskInfoDataAsset* TargetShootingTaskInfoDataAsset = Cast<UST_TargetShootingTaskInfoDataAsset>(QuestInfoDataAsset);
	if (!IsValid(TargetShootingTaskInfoDataAsset))
	{
		UE_LOG(LogTemp, Warning, TEXT("UST_TargetShootingQuest::FillQuestInfo: TargetShootingTaskInfoDataAsset is not valid!"));
		return;
	}

	AddedTimeForTarget = TargetShootingTaskInfoDataAsset->AddedTimeForTarget;
	AddedScoreForTarget = TargetShootingTaskInfoDataAsset->AddedScoreForTarget;
	ScoreToWin = TargetShootingTaskInfoDataAsset->ScoreToWin;
}

void UST_QuestTask_TargetShooting::OnTaskStarted()
{
	Super::OnTaskStarted();

	CurrentScore = 0;
	
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogTemp, Warning, TEXT("UST_TargetShootingQuest::OnTaskStarted: World is not valid!"));
		return;
	}

	UObjectSpawnSubsystem* ObjectSpawnSubsystem = World->GetSubsystem<UObjectSpawnSubsystem>();
	if (!IsValid(ObjectSpawnSubsystem))
	{
		UE_LOG(LogTemp, Warning, TEXT("UST_TargetShootingQuest::OnTaskStarted: ObjectSpawnSubsystem is not valid!"));
		return;
	}

	ObjectSpawnSubsystem->FindOrAddObjectSpawnManager(ESpawnObjectType::Target, GetQuestProviderObject());
	FOnObjectSpawnedDelegate* OnObjectSpawnedDelegate = ObjectSpawnSubsystem->GetManagerObjectSpawnedDelegate(ESpawnObjectType::Target, GetQuestProviderObject());
	if (OnObjectSpawnedDelegate == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UST_TargetShootingQuest::OnTaskStarted: OnObjectSpawnedDelegate is not valid!"));
		return;
	}

	OnObjectSpawnedDelegate->AddUObject(this, &ThisClass::OnTargetSpawned);
	ObjectSpawnSubsystem->SpawnObject(ESpawnObjectType::Target, GetQuestProviderObject());
}

void UST_QuestTask_TargetShooting::OnTargetDestroyed(AActor* DestroyedActor)
{
	CurrentScore += AddedScoreForTarget;
	AddRemainingTime(AddedTimeForTarget);

	OnScoreChangedDelegate.Broadcast(CurrentScore);

	if (CurrentScore >= ScoreToWin && ScoreToWin > 0)
	{
		FinishTask(EQuestTaskCompleteResult::Succeeded);
		return;
	}
	
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogTemp, Warning, TEXT("UST_TargetShootingQuest::OnTaskStarted: World is not valid!"));
		return;
	}

	UObjectSpawnSubsystem* ObjectSpawnSubsystem = World->GetSubsystem<UObjectSpawnSubsystem>();
	if (!IsValid(ObjectSpawnSubsystem))
	{
		UE_LOG(LogTemp, Warning, TEXT("UST_TargetShootingQuest::OnTaskStarted: ObjectSpawnSubsystem is not valid!"));
		return;
	}

	ObjectSpawnSubsystem->SpawnObject(ESpawnObjectType::Target, GetQuestProviderObject());
}

void UST_QuestTask_TargetShooting::OnTargetSpawned(AActor* InTargetActor)
{
	AST_ShootTarget* Target = Cast<AST_ShootTarget>(InTargetActor);
	if (!Target)
	{
		UE_LOG(LogTemp, Warning, TEXT("UST_TargetShootingQuest::OnTargetSpawned: Failed to spawn valid target!"));
		return;
	}

	Target->OnDestroyed.AddDynamic(this, &ThisClass::OnTargetDestroyed);
}
