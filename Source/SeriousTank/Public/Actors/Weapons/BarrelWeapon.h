#pragma once

#include "Actors/Weapons/BaseWeapon.h"
#include "BarrelWeapon.generated.h"

class AST_BaseProjectile;

UCLASS()
class SERIOUSTANK_API ABarrelWeapon : public ABaseWeapon
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AST_BaseProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ReloadingTime;

private:
	bool IsFireForced;
	bool IsWeaponReloading;

public:
	ABarrelWeapon();

	virtual void StartFire() override;
	virtual void StopFire() override;

	virtual bool IsReloading() const override;
	virtual float GetReloadingRemainingTime() const override;

private:
	void Shoot();

	void StartReloading();
	void StopReloading();
};
