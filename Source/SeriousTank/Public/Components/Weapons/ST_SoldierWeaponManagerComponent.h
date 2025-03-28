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
	void SetupSockets(const FName InRightHandSocketName, const FName InLeftHandSocketName, const FName InSecondWeaponSocketName);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    virtual void AddWeapon(AST_BaseWeapon* NewWeapon) override;
	virtual void OnWeaponSwitched(int32 PrevWeaponIndex, int32 NewWeaponIndex) override;
	
	virtual void OnOwnerPawnPossessed(AController* OldController) override;
	virtual void OnOwnerPawnUnPossessed(AController* OldController) override;

	void OnWeaponFired();
    
private:
    void OnWeaponReloadingStarted();
};
