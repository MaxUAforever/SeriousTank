#pragma once

#include "Components/ActorComponent.h"
#include "ST_BaseWeaponsManagerComponent.generated.h"

class AController;
class APawn;
class AST_BaseWeapon;

struct FWeaponAdditionalInfo
{
	FTimerHandle ReloadingTimerHandle;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SERIOUSTANK_API UST_BaseWeaponsManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FOnWeaponFired, AST_BaseWeapon*)
	FOnWeaponFired OnWeaponFiredDelegate;

	DECLARE_MULTICAST_DELEGATE_TwoParams(FWeaponAdded, int32, AST_BaseWeapon*)
	FWeaponAdded OnWeaponAdded;

	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnWeaponReloadingStartedDelegate, int32, AST_BaseWeapon*)
    FOnWeaponReloadingStartedDelegate OnWeaponReloadingStartedDelegate;
    
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnWeaponReloadingFinishedDelegate, int32, AST_BaseWeapon*)
	FOnWeaponReloadingFinishedDelegate OnWeaponReloadingFinishedDelegate;

	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnWeaponSwitchingStartedDelegate, int32 /*PreviousWeaponIndex*/, int32 /*NewWeaponIndex*/)
	FOnWeaponSwitchingStartedDelegate OnWeaponSwitchingStartedDelegate;

	DECLARE_MULTICAST_DELEGATE(FOnWeaponSwitchingCompletedDelegate)
	FOnWeaponSwitchingCompletedDelegate OnWeaponSwitchingCompletedDelegate;

public:
	UST_BaseWeaponsManagerComponent();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type Reason) override;

public:
	bool StartFire();
	void StopFire();

    void ReloadCurrentWeapon();
	void InterruptReloading();

	bool IsReloadingWeapon(int32 WeaponIndex) const;
	float GetWeaponReloadingTime(int32 WeaponIndex) const;
	float GetWeaponTotalReloadingTime(int32 WeaponIndex) const;

	bool IsWeaponSwitching() const { return bIsWeaponSwitching; }
	bool StartSwitchingWeapon(int32 WeaponIndex);
	
	float GetWeaponSwitchingTime() { return WeaponSwitchingTime; }
	void SetWeaponSwitchingTime(float InWeaponSwitchingTime) { WeaponSwitchingTime = InWeaponSwitchingTime; }

	const TArray<AST_BaseWeapon*>& GetWeapons() const { return Weapons; }
	AST_BaseWeapon* GetCurrentWeapon() const;
	AST_BaseWeapon* GetWeapon(int32 WeaponIndex) const;

	FORCEINLINE int32 GetCurrentWeaponIndex() const { return CurrentWeaponIndex; }

protected:
    virtual void AddWeapon(AST_BaseWeapon* NewWeapon);
    
	virtual void OnWeaponSwitchingStarted(int32 PrevWeaponIndex, int32 NewWeaponIndex) {};
	virtual void OnWeaponSwitchingCompleted() {};

private:
	void CompleteWeaponSwitching();

private:
	UFUNCTION()
	void OnControllerChanged(APawn* Pawn, AController* OldController, AController* NewController);

	void FinishReloading(int32 WeaponIndex);

	void OnOwnerPawnPossessed(AController* NewController);
	void OnOwnerPawnUnPossessed(AController* OldController);

	void OnAmmoCountChanged(int32 NewAmmoCount, int32 WeaponIndex);

protected:
	const FWeaponAdditionalInfo* GetWeaponInfo(int32 WeaponIndex) const;

private:
	UPROPERTY(EditAnywhere, Category = "Settings")
	float WeaponSwitchingTime = 0.f;

	TArray<TObjectPtr<AST_BaseWeapon>> Weapons;
	TArray<FWeaponAdditionalInfo> WeaponsInfo;

	int32 CurrentWeaponIndex;

	bool bIsWeaponSwitching;
	FTimerHandle WeaponSwitchingTimerHandle;

//#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere)
	TSubclassOf<AST_BaseWeapon> OverriddenFirstWeapon;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AST_BaseWeapon> OverriddenSecondWeapon;
//#endif
};
