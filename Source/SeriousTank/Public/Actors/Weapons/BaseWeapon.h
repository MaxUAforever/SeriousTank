#pragma once

#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

class UArrowComponent;
class USoundCue;

UCLASS(Abstract)
class SERIOUSTANK_API ABaseWeapon : public AActor
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE(FReloadingStarted)
	FReloadingStarted OnReloadingStarted;

    DECLARE_MULTICAST_DELEGATE_OneParam(FOnAmmoCountChanged, int32)
    FOnAmmoCountChanged OnAmmoCountChanged;
    
protected:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere)
	UArrowComponent* ShootingArrowComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	USoundCue* ShootSound;

	UPROPERTY(VisibleAnywhere, Category = "TimerHandler")
	FTimerHandle ReloadTimerHandler;

	UPROPERTY(EditAnywhere, Category = "Ammo")
	float ReloadingTime;
    
	UPROPERTY(EditAnywhere, Category = "Ammo")
	int32 TotalAmmoCount;
    
private:
	bool bIsFireForced;
	bool bIsWeaponReloading;
    
public:
	ABaseWeapon();

	bool IsFireForced() const { return bIsFireForced; };
	bool IsReloading() const { return bIsWeaponReloading; };
    
    float GetTotalReloadingTime() const { return ReloadingTime; };
	float GetReloadingRemainingTime() const;

	int32 GetTotalAmmoCount() const { return TotalAmmoCount; };
	void SetTotalAmmoCount(int32 NewAmmoCount);
    
 	void StartFire();
	void StopFire();

	void StartReloading();
    
    virtual void AttachToVehicleComponent(USceneComponent* ParentVehicleComponent);
    virtual void SetWeaponEnabled(bool bIsEnabled) {};
    
protected:
	void StopReloading();

	bool CanShoot() const;

	virtual void Shoot() {};

	virtual void StartShooting() {};
	virtual void StopShooting() {};

	virtual void FinishReloading() {};
};
