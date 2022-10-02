#pragma once

#include "Actors/Weapons/BaseWeapon.h"

#include "Actors/Projectiles/BaseProjectile.h"

#include "BarrelWeapon.generated.h"

UCLASS()
class SERIOUSTANK_API ABarrelWeapon : public ABaseWeapon
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<ABaseProjectile> ProjectileClass;

	UPROPERTY(VisibleAnywhere, Category = "TimerHandler")
	FTimerHandle ReloadTimerHandler;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ReloadingTime;

private:
	bool IsFireForced;
	bool IsReloading;

public:
	ABarrelWeapon();

	virtual void StartFire() override;
	virtual void StopFire() override;

private:
	void Shoot();

	void StartReloading();
	void StopReloading();
};
