#pragma once

#include "Components/ArrowComponent.h"

#include "Actors/Weapons/BaseWeapon.h"

#include "ST_WeaponSocketComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SERIOUSTANK_API UST_WeaponSocketComponent : public UArrowComponent
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FWeaponAdded, ABaseWeapon*)
    FWeaponAdded OnWeaponAdded;

protected:
    UPROPERTY(EditAnywhere, Category = "Weapon")
    TSubclassOf<ABaseWeapon> DefaultWeaponClass;
    
private:
	ABaseWeapon* Weapon;

public:
	ABaseWeapon* GetWeapon() const { return Weapon; }
	ABaseWeapon* SetWeapon(TSubclassOf<ABaseWeapon> WeaponClass);
    
    TSubclassOf<ABaseWeapon> GetDefaultWeaponClass() const { return DefaultWeaponClass; };
    void DestroyWeapon();
};
