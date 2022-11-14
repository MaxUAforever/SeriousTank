#pragma once

#include "Blueprint/UserWidget.h"
#include "ST_UserSettings.generated.h"

class UButton;
class UUserWidget;
class UWidgetSwitcher;

UCLASS()
class SERIOUSTANK_API UST_UserSettings : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* AudioTabButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* VideoTabButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UUserWidget* AudioSettingsWidget;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UUserWidget* VideoSettingsWidget;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UWidgetSwitcher* SettingWidgetsSwitcher;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ApplyButton;

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnAudioTabButtonClicked();

	UFUNCTION()
	void OnVideoTabButtonClicked();

	UFUNCTION()
	void OnApplyButtonClicked();
};
