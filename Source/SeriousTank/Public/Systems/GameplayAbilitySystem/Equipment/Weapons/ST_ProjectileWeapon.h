#pragma once

#include "Systems/GameplayAbilitySystem/Equipment/Weapons/ST_WeaponBase.h"
#include "ST_ProjectileWeapon.generated.h"

class AST_BaseProjectile;
class USpringArmComponent;

UCLASS()
class SERIOUSTANK_API AST_ProjectileWeapon : public AST_WeaponBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere)
    USpringArmComponent* AimSpringArm;
    
    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* AimMesh;
   
	UPROPERTY(EditAnywhere)
	TSubclassOf<AST_BaseProjectile> ProjectileClass;

    UPROPERTY(EditAnywhere)
    float AimLocationOffset;

private:
	int32 CurrentAmmoCount;

public:
	AST_ProjectileWeapon();

protected:
	virtual void BeginPlay() override;

public:
	FORCEINLINE TSubclassOf<AST_BaseProjectile> GetProjectileClass() const { return ProjectileClass; };

	virtual bool CheckCost() override;
	virtual void ApplyCost() override;

	virtual void HandleAbilityActivated() override;
	virtual void HandleAbilityEnded() override;
};
