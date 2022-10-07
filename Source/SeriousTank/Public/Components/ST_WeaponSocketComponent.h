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
	UPROPERTY(EditAnywhere, Category="Weapon")
	TSubclassOf<ABaseWeapon> WeaponClass;

private:
	ABaseWeapon* Weapon;

public:
	ABaseWeapon* GetWeapon()
	{
		return Weapon;
	}

protected:
	virtual void BeginPlay() override;
};
