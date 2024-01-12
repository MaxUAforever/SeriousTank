#pragma once

#include "Components/Weapons/ST_BaseWeaponsManagerComponent.h"
#include "ST_SoldierWeaponManagerComponent.generated.h"

UCLASS()
class SERIOUSTANK_API UST_SoldierWeaponManagerComponent : public UST_BaseWeaponsManagerComponent
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditdefaultsOnly)
	FName RightHandSocketName;

	UPROPERTY(EditdefaultsOnly)
	FName LeftHandSocketName;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void OnWeaponSwitched(int32 PrevWeaponIndex, int32 NewWeaponIndex) override;
	
	void OnWeaponFired();
};
