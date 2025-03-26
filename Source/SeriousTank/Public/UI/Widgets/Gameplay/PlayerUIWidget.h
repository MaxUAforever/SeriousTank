#pragma once

#include "Blueprint/UserWidget.h"
#include "PlayerUIWidget.generated.h"

class UCanvasPanel;
class UST_HealthUserWidget;
class UST_VMHealth;
class UST_WeaponReloadingWidget;
class UST_PreStartCountdownWidget;
class UST_VMActiveQuestTask;

UCLASS()
class SERIOUSTANK_API UPlayerUIWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

private:
	void OnTrackedTaskWidgetChanged(UUserWidget* InTrackedTaskWidget);

protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> RootCanvasPanel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UST_WeaponReloadingWidget> FirstWeaponReloadingWidget;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UST_WeaponReloadingWidget> SecondWeaponReloadingWidget;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UST_WeaponReloadingWidget> ThirdWeaponReloadingWidget;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UST_HealthUserWidget> HealthUserWidget;

	UPROPERTY(EditAnyWhere, meta = (BindWidget))
	TObjectPtr<UST_PreStartCountdownWidget> PreStartCountdownWidget;

private:
	UPROPERTY()
	TObjectPtr<UUserWidget> TrackedQuestTaskWidget;

	UPROPERTY()
	TObjectPtr<UST_VMHealth> HealthViewModel;

	UPROPERTY()
	TObjectPtr<UST_VMActiveQuestTask> ActiveQuestTaskViewModel;
};
