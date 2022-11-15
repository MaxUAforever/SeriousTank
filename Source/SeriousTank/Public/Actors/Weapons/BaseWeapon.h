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

	UPROPERTY(EditAnywhere, Category = "Reloading")
	float ReloadingTime;

private:
	bool bIsFireForced;
	bool bIsWeaponReloading;

public:
	ABaseWeapon();

	bool IsFireForced() const { return bIsFireForced; };
	bool IsReloading() const { return bIsWeaponReloading; };
	float GetReloadingRemainingTime() const;

 	void StartFire();
	void StopFire();

	void StartReloading();

protected:
	void StopReloading();

	virtual void StartShooting() {};
	virtual void StopShooting() {};

	virtual void FinishReloading() {};
};
