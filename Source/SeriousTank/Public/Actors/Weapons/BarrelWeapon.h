#pragma once

#include "Actors/Weapons/ST_FireArmWeapon.h"
#include "BarrelWeapon.generated.h"

class AST_BaseProjectile;

UCLASS()
class SERIOUSTANK_API ABarrelWeapon : public AST_FireArmWeapon
{
	GENERATED_BODY()

protected:
	virtual void StartShooting() override;
};
