#pragma once

#include "Blueprint/UserWidget.h"
#include "ST_WeaponReloadingWidget.generated.h"

class AST_BaseWeapon;
class UBorder;
class UProgressBar;
class UTextBlock;
class UST_BaseWeaponsManagerComponent;

UCLASS()
class SERIOUSTANK_API UST_WeaponReloadingWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
    UTextBlock* WeaponIndexBlock;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* ReloadingProgressbar;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
    UBorder* SelectionBorder;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
    UTextBlock* ClipAmmoCountBlock;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
    UTextBlock* AmmoDelimiterBlock;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
    UTextBlock* TotalAmmoCountBlock;
    
	// Timer determines how often the reloading value should be updated in Widget.
	UPROPERTY(VisibleAnywhere, Category = "TimerHandler")
	FTimerHandle WeaponReloadingRefreshRateHandler;

	UPROPERTY(EditAnywhere)
	int32 WeaponIndex;

private:
	float ReloadingRefreshRate = 0.02f;

	FDelegateHandle OnWeaponAddedDelegateHandle;
    FDelegateHandle OnWeaponRemovedDelegateHandle;
	FDelegateHandle OnWeaponSwitchedDelegateHandle;

	FDelegateHandle OnReloadingStartedDelegateHandle;
	FDelegateHandle OnAmmoCountChangedDelegateHandle;
	FDelegateHandle OnClipAmmoCountChangedDelegateHandle;

    UST_BaseWeaponsManagerComponent* WeaponsManagerComponent;

protected:
	virtual void NativeConstruct() override;

private:
	void OnWeaponAdded(int32 InWeaponIndex, AST_BaseWeapon* Weapon);
	void OnWeaponRemoved(int32 InWeaponIndex, AST_BaseWeapon* Weapon);

	void OnWeaponReloadingStarted(int32 WeaponIndex, AST_BaseWeapon* Weapon);

    void OnWeaponSelected(int32 PreviousWeaponIndex, int32 NewWeaponIndex);
    void UpdateTotalAmmoCount(int32 OldAmmoCount, int32 TotalAmmoCount);
    void UpdateClipAmmoCount(int32 ClipAmmoCount);
    
	UFUNCTION()
	void OnPawnChanged(APawn* OldPawn, APawn* NewPawn);

	void UpdateReloadingProgressbar();
};
