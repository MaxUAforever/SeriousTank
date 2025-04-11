#pragma once

#include "UI/InteractionUserWidget.h"
#include "InterruptibleInteractionUserWidget.generated.h"

class UBaseInteractionAction;
class UBaseInterruptibleAction;
class UProgressBar;

/**
 * Widget for interruptible interaction with activation progress bar.
 */
UCLASS()
class PLAYERINTERACTIONSUBSYSTEM_API UInterruptibleInteractionUserWidget : public UInteractionUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void OnInteractionActionStarted(UBaseInteractionAction* InteractionAction) override;
	virtual void OnInteractionActionStopped() override;

	virtual void BeginDestroy() override;

private:
	void UpdateActivationProgress();

protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UProgressBar> ActivationProgressBar;

private:
	TObjectPtr<UBaseInterruptibleAction> CurrentAction;

	FTimerHandle ProgressUpdateTimerHandle;
};
