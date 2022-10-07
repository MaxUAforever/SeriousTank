#pragma once

#include "Blueprint/UserWidget.h"
#include "ST_WeaponReloadingWidget.generated.h"

class UTextBlock;

UCLASS()
class SERIOUSTANK_API UST_WeaponReloadingWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* WeaponReloadingBlock;

	// Timer determines how often the reloading value should be updated in Widget.
	UPROPERTY(VisibleAnywhere, Category = "TimerHandler")
	FTimerHandle WeaponReloadingRefreshRateHandler;

	UPROPERTY(EditAnywhere)
	int32 WeaponIndex;

private:
	float ReloadingRefreshRate = 0.05f;

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnWeaponAdded(ABaseWeapon* Weapon);

	UFUNCTION()
	void OnWeaponReloadingStarted(ABaseWeapon* Weapon);

	void UpdateReloadingTextBlock(ABaseWeapon* Weapon);
};
