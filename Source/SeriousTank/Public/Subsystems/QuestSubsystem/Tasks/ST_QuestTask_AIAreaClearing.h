#pragma once

#include "QuestSubsystem/Public/Tasks/BaseQuestTask.h"
#include "ST_QuestTask_AIAreaClearing.generated.h"

class UST_HealthComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAreaClearingWaveChangedDelegate, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAreaClearingWaveEnemyAmountChangedDelegate, int32);

UCLASS()
class SERIOUSTANK_API UST_QuestTask_AIAreaClearing : public UBaseQuestTask
{
    GENERATED_BODY()

public:
    FOnAreaClearingWaveChangedDelegate OnAreaClearingWaveChangedDelegate;
    FOnAreaClearingWaveEnemyAmountChangedDelegate OnAreaClearingWaveEnemyAmountChangedDelegate;

public:
    virtual void FillTaskInfo(const UQuestTaskInfoDataAsset* QuestInfoDataAsset) override;

    int32 GetCurrentWaveIndex() const { return CurrentWaveIndex; }
    int32 GetCurrentWaveAIAmount() const { return CurrentWaveAIAmount; }

protected:
	virtual void OnTaskStarted() override;
    virtual void OnTaskCompleted(EQuestTaskCompleteResult CompleteResult) override;

private:
    void StartWave(int32 WaveIndex);

    void OnEnemySpawned(AActor* InTargetActor);
    void OnEnemyDestroyed();

    void SpawnEnemy();

private:
    TArray<int32> EnemiesWavesAIAmount;

    int32 CurrentWaveIndex;
    int32 CurrentWaveAIAmount;

    FDelegateHandle OnObjectSpawnedDelegateHandle;
    TMap<TObjectPtr<UST_HealthComponent>, FDelegateHandle> EnemyDestroyedDelegateHandleMap;
};
