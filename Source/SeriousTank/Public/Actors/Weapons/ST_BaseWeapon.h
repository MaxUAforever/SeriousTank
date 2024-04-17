// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ST_BaseWeapon.generated.h"

class UArrowComponent;
class USoundCue;
enum class EWeaponOwnerType : uint8;

UCLASS()
class SERIOUSTANK_API AST_BaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:
	DECLARE_DELEGATE(FOnShootDone)
	FOnShootDone OnShootDone;

	DECLARE_MULTICAST_DELEGATE(FReloadingStarted)
	FReloadingStarted OnReloadingStarted;

    DECLARE_MULTICAST_DELEGATE_OneParam(FOnAmmoCountChanged, int32)
    FOnAmmoCountChanged OnAmmoCountChanged;

protected:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere)
	UArrowComponent* ShootingArrowComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	USoundCue* ShootSound;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	EWeaponOwnerType WeaponOwnerType;

	UPROPERTY(VisibleAnywhere, Category = "TimerHandler")
	FTimerHandle ReloadTimerHandler;

	UPROPERTY(EditAnywhere, Category = "Ammo")
	float ReloadingTime;
    
	UPROPERTY(EditAnywhere, Category = "Ammo")
	int32 TotalAmmoCount;
    
private:
	bool bIsFireForced;
	bool bIsWeaponReloading;
    
	bool bShouldBeDestroyedWithActor = true;

public:
	AST_BaseWeapon();

	bool IsFireForced() const { return bIsFireForced; };
	bool IsReloading() const { return bIsWeaponReloading; };
    
    float GetTotalReloadingTime() const { return ReloadingTime; };
	float GetReloadingRemainingTime() const;

	int32 GetTotalAmmoCount() const { return TotalAmmoCount; };
	void SetTotalAmmoCount(int32 NewAmmoCount);
    
	EWeaponOwnerType GetWeaponOwnerType() const { return WeaponOwnerType; }
 	
	void StartFire();
	void StopFire();
    
    void ForceReload();
	void StartReloading();
	void InterruptReloading();

    virtual void AttachToParentComponent(USceneComponent* InParentComponent, FName SocketName = NAME_None, bool bInShouldBeDestroyedWithActor = true);
    virtual void SetWeaponEnabled(bool bIsEnabled) {};
    
	void SetHidden(bool bIsHidden);

	bool CanShoot() const;
	
	virtual bool IsReloadingNeeded() const { return false; };

protected:
	void StopReloading();

	virtual void Shoot() {};

	virtual void StartShooting() {};
	virtual void StopShooting() {};

	virtual bool CanReload() const { return false; };
	virtual void FinishReloading() {};
    
private:
    UFUNCTION()
    void OnParentDestroyed(AActor* DestroyedOwner);

};
