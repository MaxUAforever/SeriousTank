#pragma once

#include "QuestSubsystem/Public/UI/ViewModel/ViewModelBase.h"
#include "ST_VMAreaClearing.generated.h"

class UST_AreaClearingTaskWidget;
class UST_QuestTask_AIAreaClearing;

UCLASS()
class SERIOUSTANK_API UST_VMAreaClearing : public UViewModelBase
{
	GENERATED_BODY()

public:
	virtual void Initialize(UUserWidget* RelatedWidget) override;
	virtual void Deinitialize() override;

private:
	void OnWaveChanged(int32 InWaveIndex);
	void OnEnemyAmountChanged(int32 InEnemyAmount);

private:
	TObjectPtr<UST_QuestTask_AIAreaClearing> AreaClearingTask;
	TObjectPtr<UST_AreaClearingTaskWidget> AreaClearingTaskWidget;

	FDelegateHandle OnWaveChangedDelegateHandle;
	FDelegateHandle OnEnemyAmountChangedDelegate;
};
