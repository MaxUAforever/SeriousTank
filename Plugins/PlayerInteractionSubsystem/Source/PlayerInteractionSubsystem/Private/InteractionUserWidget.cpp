#include "InteractionUserWidget.h"

#include "Components/TextBlock.h"

void UInteractionUserWidget::SetInteractionButtonCaption(FText InButtonText)
{
	if (InteractionButtonCaption)
	{
		InteractionButtonCaption->SetText(InButtonText);
	}
}

void UInteractionUserWidget::SetActionNameCaption(FText InActionDescriptionText)
{
	if (ActionDescriptionCaption)
	{
		ActionDescriptionCaption->SetText(InActionDescriptionText);
	}
}
