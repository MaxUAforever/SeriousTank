#pragma once

#include "UI/ViewModel/ViewModelBase.h"
#include "Engine/TimerHandle.h"
#include "ViewModelTimeLimitedTask.generated.h"

class UTimeLimitedTaskWidget;
class UTimeLimitedQuestTask;
class UUserWidget;

UCLASS()
class QUESTSUBSYSTEM_API UViewModelTimeLimitedTask : public UViewModelBase
{
	GENERATED_BODY()

public:
	UViewModelTimeLimitedTask();

	virtual void Initialize(UUserWidget* RelatedWidget) override;
	virtual void Deinitialize() override;

private:
	void UpdateTime();

private:
	FTimerHandle TimeRefreshHandler;
	float TimeRefreshRate;

private:
	TObjectPtr<UTimeLimitedTaskWidget> TimeLimitedTaskWidget;

	TObjectPtr<const UTimeLimitedQuestTask> TrackedTimeLimitedTask;
};
