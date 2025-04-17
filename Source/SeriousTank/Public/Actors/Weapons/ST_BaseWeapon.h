// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ST_BaseWeapon.generated.h"

class UArrowComponent;
class USoundCue;
class UInteractionComponent;
enum class EWeaponOwnerType : uint8;

UCLASS()
class SERIOUSTANK_API AST_BaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:
	DECLARE_DELEGATE(FOnShootDone)
	FOnShootDone OnShootDone;

	DECLARE_MULTICAST_DELEGATE(FReloadingStartedDelegate)
	FReloadingStartedDelegate OnReloadingStartedDelegate;

    DECLARE_MULTICAST_DELEGATE_OneParam(FOnAmmoCountChangedDelegate, int32)
    FOnAmmoCountChangedDelegate OnAmmoCountChangedDelegate;

public:
	AST_BaseWeapon();

	bool IsFireForced() const { return bIsFireForced; };
	bool IsReloading() const { return bIsWeaponReloading; };
    
    float GetTotalReloadingTime() const { return ReloadingTime; };
	
	int32 GetMaxAmmoCount() const { return MaxAmmoCount; }
	int32 GetTotalAmmoCount() const { return TotalAmmoCount; };
	
	void SetTotalAmmoCount(int32 NewAmmoCount);
	void AddAmmo(int32 AddedAmmoCount);

	void SetWeaponEquipped(bool bInIsEquipped);

	bool StartFire();
	void StopFire();

	bool StartReloading();
	void CompleteReloading();
	void InterruptReloading();

	float GetDefaultReloadingTime() const { return ReloadingTime; }
	
	EWeaponOwnerType GetWeaponOwnerType() const { return WeaponOwnerType; }

	bool IsEnabled() const { return bIsEnabled; }
	void SetEnabled(bool bInIsEnabled);
	void SetHidden(bool bIsHidden);

public:
	virtual bool CanShoot() const;
	virtual bool CanReload() const;
	virtual bool IsReloadingNeeded() const { return false; };

	virtual void AttachToParentComponent(USceneComponent* InParentComponent, FName SocketName = NAME_None, bool bInShouldBeDestroyedWithActor = true);

protected:
	virtual void OnReloadingStarted() {};
	virtual void OnReloadingCompleted() {};
	virtual void OnReloadingInterrupted() {};

	virtual void Shoot() {};

	virtual void StartShooting() {};
	virtual void StopShooting() {};

	virtual void OnSetWeaponEnabled(bool bInIsEnabled) {};
    
private:
    UFUNCTION()
    void OnParentDestroyed(AActor* DestroyedOwner);

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> SceneComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UArrowComponent> ShootingArrowComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInteractionComponent> PickUpInteractionComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	TObjectPtr<USoundCue> ShootSound;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	EWeaponOwnerType WeaponOwnerType;

	UPROPERTY(EditAnywhere, Category = "Ammo")
	float ReloadingTime;

	UPROPERTY(EditAnywhere, Category = "Ammo")
	int32 TotalAmmoCount;

	UPROPERTY(EditAnywhere, Category = "Ammo")
	int32 MaxAmmoCount;

private:
	bool bIsFireForced;
	bool bIsWeaponReloading;

	bool bIsEnabled;

	bool bShouldBeDestroyedWithActor = true;
};
