#pragma once

#include "Actors/Weapons/ST_FireArmWeapon.h"
#include "ST_BarrelWeapon.generated.h"

UCLASS()
class SERIOUSTANK_API AST_BarrelWeapon : public AST_FireArmWeapon
{
	GENERATED_BODY()

protected:
	virtual void StartShooting() override;
};
