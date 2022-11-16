#pragma once

#include "Actors/Weapons/BaseWeapon.h"
#include "ST_FireArmWeapon.generated.h"

class AST_BaseProjectile;

UCLASS(Abstract)
class SERIOUSTANK_API AST_FireArmWeapon : public ABaseWeapon
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AST_BaseProjectile> ProjectileClass;

protected:
	virtual void Shoot() override;

	virtual void FinishReloading() override;
};
