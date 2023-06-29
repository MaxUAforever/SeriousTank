#pragma once

#include "Components/ArrowComponent.h"
#include "Actors/Weapons/ST_BaseWeapon.h"
#include "ST_WeaponSocketComponent.generated.h"

class AST_WeaponBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SERIOUSTANK_API UST_WeaponSocketComponent : public UArrowComponent
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FWeaponAdded, AST_BaseWeapon*)
    FWeaponAdded OnWeaponAdded;

protected:
    UPROPERTY(EditAnywhere, Category = "Weapon")
    TSubclassOf<AST_WeaponBase> DefaultWeaponClass;
    
private:
	AST_WeaponBase* Weapon;

protected:
	virtual void OnChildAttached(USceneComponent* ChildComponent) override;

public:
	AST_WeaponBase* GetWeapon() const { return Weapon; }
	
	TSubclassOf<AST_WeaponBase> GetDefaultWeaponClass() const { return DefaultWeaponClass; };
    
	void DestroyWeapon();
};
