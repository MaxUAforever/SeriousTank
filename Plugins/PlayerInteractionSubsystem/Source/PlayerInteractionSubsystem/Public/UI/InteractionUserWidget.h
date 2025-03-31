#pragma once

#include "Blueprint/UserWidget.h"
#include "InteractionUserWidget.generated.h"

class UTextBlock;

/**
 * Widget that can be used to dispaly user that he can interact with the object.
 */
UCLASS()
class PLAYERINTERACTIONSUBSYSTEM_API UInteractionUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetInteractionButtonCaption(FText InButtonText);
	void SetActionNameCaption(FText InActionDescriptionText);

protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* InteractionButtonCaption;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* ActionDescriptionCaption;
};
