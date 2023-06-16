#pragma once

#include "Components/ArrowComponent.h"
#include "Actors/Weapons/ST_BaseWeapon.h"
#include "ST_WeaponSocketComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SERIOUSTANK_API UST_WeaponSocketComponent : public UArrowComponent
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FWeaponAdded, AST_BaseWeapon*)
    FWeaponAdded OnWeaponAdded;

protected:
    UPROPERTY(EditAnywhere, Category = "Weapon")
    TSubclassOf<AST_BaseWeapon> DefaultWeaponClass;
    
private:
	AST_BaseWeapon* Weapon;

public:
	AST_BaseWeapon* GetWeapon() const { return Weapon; }
	AST_BaseWeapon* SetWeapon(TSubclassOf<AST_BaseWeapon> WeaponClass);
    
    TSubclassOf<AST_BaseWeapon> GetDefaultWeaponClass() const { return DefaultWeaponClass; };
    void DestroyWeapon();
};
