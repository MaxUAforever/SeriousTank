#pragma once

#include "Actors/Weapons/ST_BaseWeapon.h"
#include "ST_FireArmWeapon.generated.h"

class AST_BaseProjectile;
class USpringArmComponent;

UCLASS(Abstract)
class SERIOUSTANK_API AST_FireArmWeapon : public AST_BaseWeapon
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
    
    virtual void AttachToParentComponent(USceneComponent* InParentComponent, FName SocketName = NAME_None, bool bInShouldBeDestroyedWithActor = true) override;
    virtual void OnSetWeaponEnabled(bool bIsEnabled) override;
    
protected:
	virtual void Shoot() override;

	virtual void FinishReloading() override;
};
