#pragma once

#include "Components/ActorComponent.h"
#include "ST_BaseWeaponsManagerComponent.generated.h"

class AController;
class APawn;
class AST_BaseWeapon;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SERIOUSTANK_API UST_BaseWeaponsManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FOnWeaponFired, AST_BaseWeapon*)
	FOnWeaponFired OnWeaponFiredDelegate;

	DECLARE_MULTICAST_DELEGATE_TwoParams(FWeaponAdded, int32, AST_BaseWeapon*)
	FWeaponAdded OnWeaponAdded;

    DECLARE_MULTICAST_DELEGATE_OneParam(FOnWeaponReloadingStartedDelegate, AST_BaseWeapon*)
    FOnWeaponReloadingStartedDelegate OnWeaponReloadingStartedDelegate;
    
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnWeaponSwitched, int32 /*PreviousWeaponIndex*/, int32 /*NewWeaponIndex*/)
	FOnWeaponSwitched OnWeaponSwitchedDelegate;

protected:
	TArray<AST_BaseWeapon*> Weapons;
	int32 CurrentWeaponIndex;

public:
	UST_BaseWeaponsManagerComponent();

protected:
	virtual void BeginPlay() override;

public:
	void StartFire();
	void StopFire();

    void Reload();
	void InterruptReloading();

	bool SwitchWeapon(int32 WeaponIndex);

	const TArray<AST_BaseWeapon*>& GetWeapons() const { return Weapons; }
	AST_BaseWeapon* GetCurrentWeapon() const;
	AST_BaseWeapon* GetWeapon(int32 WeaponIndex) const;

	FORCEINLINE int32 GetCurrentWeaponIndex() const { return CurrentWeaponIndex; }

protected:
    virtual void AddWeapon(AST_BaseWeapon* NewWeapon);
    
	virtual void OnWeaponSwitched(int32 PrevWeaponIndex, int32 NewWeaponIndex) {};

	virtual void OnOwnerPawnPossessed(AController* NewController) {};
	virtual void OnOwnerPawnUnPossessed(AController* OldController) {};

private:
	UFUNCTION()
	void OnControllerChanged(APawn* Pawn, AController* OldController, AController* NewController);
};
