#pragma once

#include "Actors/Weapons/ST_FireArmWeapon.h"
#include "ST_BarrelWeapon.generated.h"

UCLASS()
class SERIOUSTANK_API AST_BarrelWeapon : public AST_FireArmWeapon
{
	GENERATED_BODY()

public:
	virtual bool IsReloadingNeeded() const override { return !IsCharged; };

protected:
	virtual void StartShooting() override;
	virtual void FinishReloading() override { IsCharged = true; };

	virtual bool CanReload() const { return !IsCharged && TotalAmmoCount > 0; };

private:
	bool IsCharged = true;
};
