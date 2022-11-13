#pragma once

#include "Blueprint/UserWidget.h"
#include "PlayerUIWidget.generated.h"

class UAudioComponent;
class UTextBlock;
class UST_WeaponReloadingWidget;
class USoundCue;

UCLASS()
class SERIOUSTANK_API UPlayerUIWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UST_WeaponReloadingWidget* FirstWeaponReloadingWidget;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UST_WeaponReloadingWidget* SecondWeaponReloadingWidget;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UST_WeaponReloadingWidget* ThirdWeaponReloadingWidget;

	UPROPERTY(EditAnyWhere, meta = (BindWidget))
	UTextBlock* RemainingTimeBlock;

	UPROPERTY(EditAnyWhere, meta = (BindWidget))
	UTextBlock* ScoreBlock;

	UPROPERTY(EditAnyWhere, meta = (BindWidget))
	UTextBlock* PreStartTimeBlock;

	UPROPERTY(EditAnyWhere, Category = "Audio")
	USoundCue* CountdownSound;

	UPROPERTY(EditAnyWhere, Category = "Audio")
	USoundCue* StartGameSound;

	UPROPERTY(VisibleAnyWhere, Category = "Audio")
	UAudioComponent* UISoundsComponent;

	UPROPERTY(VisibleAnywhere, Category = "TimerHandler")
	FTimerHandle TimeRefreshHandler;

	UPROPERTY(EditAnywhere, Category = "TimerHandler")
	float TimeRefreshRate;

public:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void UpdateScore(int32 NewScore);

	UFUNCTION()
	void UpdateTime();

	UFUNCTION()
	void UpdatePreStartTime(int32 NewTime);

	UFUNCTION()
	void HidePreStartTimeBlock();
};
