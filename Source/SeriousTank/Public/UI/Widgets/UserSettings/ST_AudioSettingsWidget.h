#pragma once

#include "Blueprint/UserWidget.h"
#include "ST_AudioSettingsWidget.generated.h"

class USlider;
class USoundClass;
UCLASS()
class SERIOUSTANK_API UST_AudioSettingsWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	USlider* OverallVolumeSlider;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	USlider* MusicVolumeSlider;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	USlider* SoundsVolumeSlider;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	USlider* UIVolumeSlider;

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnOverallVolumeChanged(float InValue);

	UFUNCTION()
	void OnMusicVolumeChanged(float InValue);

	UFUNCTION()
	void OnSoundsVolumeChanged(float InValue);

	UFUNCTION()
	void OnUIVolumeChanged(float InValue);
};
