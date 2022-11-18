#pragma once

#include "Actors/Weapons/BaseWeapon.h"
#include "ST_FireArmWeapon.generated.h"

class AST_BaseProjectile;
class USpringArmComponent;

UCLASS(Abstract)
class SERIOUSTANK_API AST_FireArmWeapon : public ABaseWeapon
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AST_BaseProjectile> ProjectileClass;
    
    UPROPERTY(EditAnywhere)
    USpringArmComponent* AimSpringArm;
    
    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* AimMesh;
   
    UPROPERTY(EditAnywhere)
    float AimLocationOffset;
    
public:
    AST_FireArmWeapon();
    
    virtual void AttachToVehicleComponent(USceneComponent* ParentVehicleComponent) override;
    virtual void SetWeaponEnabled(bool bIsEnabled) override;
    
protected:
	virtual void Shoot() override;

	virtual void FinishReloading() override;
};
