#pragma once

#include "UI/ViewModel/ST_ViewModelBase.h"
#include "QuestSubsystem/Public/QuestCoreTypes.h"
#include "ST_VMActiveQuestTask.generated.h"

class UQuestSubsystem;
class UQuestTaskWidgetsDataAsset;
class UUserWidget;
class UViewModelBase;

DECLARE_DELEGATE_OneParam(FOnTrackedTaskWidgetChangedDelegate, UUserWidget*);

/**
 * View model that allows to track changing of widget that is used for current 
 * tracked quest task from QuestSubsystem and notify about it. 
 */
UCLASS()
class SERIOUSTANK_API UST_VMActiveQuestTask : public UST_ViewModelBase
{
	GENERATED_BODY()
	
public:
	FOnTrackedTaskWidgetChangedDelegate OnTrackedTaskWidgetChangedDelegate;

public:
	virtual void Initialize() override;

private:
	void OnTrackedTaskWidgetChanged(UUserWidget* TaskWidget, UViewModelBase* TaskViewModel);
};
