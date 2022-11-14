#pragma once

#include "Blueprint/UserWidget.h"
#include "ST_PauseWidget.generated.h"

class UButton;

UCLASS()
class SERIOUSTANK_API UST_PauseWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ResumeButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* SettingsButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* MenuButton;

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnResumeButtonClicked();

	UFUNCTION()
	void OnSettingsButtonClicked();

	UFUNCTION()
	void OnMenuButtonClicked();
};
