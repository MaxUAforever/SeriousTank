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
    
	UPROPERTY(EditdefaultsOnly)
	FName SecondWeaponSocketName;

public:
	UST_SoldierWeaponManagerComponent();

	void SetupSockets(const FName InRightHandSocketName, const FName InLeftHandSocketName, const FName InSecondWeaponSocketName);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    virtual void OnWeaponAdded(AST_BaseWeapon* NewWeapon) override;
	virtual void OnPreWeaponRemoved(int32 WeaponIndex) override;

	virtual void OnWeaponSwitchingStarted(int32 PrevWeaponIndex, int32 NewWeaponIndex) override;

	void OnWeaponFired();
};
