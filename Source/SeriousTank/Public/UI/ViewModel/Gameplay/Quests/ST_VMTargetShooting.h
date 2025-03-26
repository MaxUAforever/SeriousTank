#pragma once

#include "QuestSubsystem/Public/UI/ViewModel/ViewModelTimeLimitedTask.h"
#include "ST_VMTargetShooting.generated.h"

class UST_TargetShootingWidget;
class UST_QuestTask_TargetShooting;

UCLASS()
class SERIOUSTANK_API UST_VMTargetShooting : public UViewModelTimeLimitedTask
{
	GENERATED_BODY()

public:
	virtual void Initialize(UUserWidget* RelatedWidget) override;
	virtual void Deinitialize() override;

private:
	void OnScoreChanged(int32 InScore);

private:
	TObjectPtr<UST_TargetShootingWidget> TargetShootingWidget;
	TObjectPtr<UST_QuestTask_TargetShooting> TargetShootingTask;

	FDelegateHandle ScoreChangedDelegateHandle;
};
