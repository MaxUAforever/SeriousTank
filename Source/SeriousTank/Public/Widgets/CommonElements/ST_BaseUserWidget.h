#pragma once

#include "Blueprint/UserWidget.h"
#include "ST_BaseUserWidget.generated.h"

class UST_MainButton;

UCLASS()
class SERIOUSTANK_API UST_BaseUserWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	TArray<UST_MainButton*> WidgetButtons;

public:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnNavigationKeyPressed();

	UFUNCTION()
	void OnButtonHovered();

	void UpdateButtonsState(bool bIsMouseEvent);
};
