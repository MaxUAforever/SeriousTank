#pragma once

#include "Components/ActorComponent.h"
#include "ST_BaseWeaponsManagerComponent.generated.h"

class AST_BaseWeapon;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SERIOUSTANK_API UST_BaseWeaponsManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE_TwoParams(FWeaponAdded, int32, AST_BaseWeapon*)
	FWeaponAdded OnWeaponAdded;

protected:
	TArray<AST_BaseWeapon*> Weapons;
	int32 CurrentWeaponIndex;

public:
	UST_BaseWeaponsManagerComponent();

	void StartFire();
	void StopFire();

	bool SwitchWeapon(int32 WeaponIndex);

	AST_BaseWeapon* GetCurrentWeapon() const;
	AST_BaseWeapon* GetWeapon(int32 WeaponIndex) const;

	FORCEINLINE int32 GetCurrentWeaponIndex() const { return CurrentWeaponIndex; }

protected:
	virtual void OnWeaponSwitched(int32 PrevWeaponIndex, int32 NewWeaponIndex) {};
};