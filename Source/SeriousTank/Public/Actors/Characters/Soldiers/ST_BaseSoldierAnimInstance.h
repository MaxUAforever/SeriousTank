#pragma once

#include "Core/Animation/ST_AnimInstance.h"
#include "ST_BaseSoldierAnimInstance.generated.h"

class AST_BaseSoldierCharacter;
class AST_BaseWeapon;
class UST_BaseWeaponsManagerComponent;
class UST_SoldierAnimDataAsset;
enum class ECharacterMovingType : uint8;
enum class ECharacterTurnSide : uint8;
enum class EHealthChangingType : uint8;

DECLARE_DELEGATE(FOnEqiupWeaponAnimationFinished);

UCLASS()
class SERIOUSTANK_API UST_BaseSoldierAnimInstance : public UST_AnimInstance
{
	GENERATED_BODY()

public:
	FOnEqiupWeaponAnimationFinished OnEqiupWeaponAnimationFinishedDelegate;

protected:
	UPROPERTY(EditDefaultsOnly)
	UST_SoldierAnimDataAsset* MontagesDataAsset;
    
	UPROPERTY(BlueprintReadOnly)
	FTransform LeftHandTransform;

	UPROPERTY(EditDefaultsOnly)
	FName RightHandBoneName;

protected:
	UPROPERTY(BlueprintReadOnly)
	ECharacterMovingType MovingType;

	UPROPERTY(BlueprintReadOnly)
	bool bIsWeaponEquipped;
    
    UPROPERTY(BlueprintReadOnly)
    bool bIsWeaponFiring;

	UPROPERTY(BlueprintReadOnly)
	bool bIsDead;

	UPROPERTY(BlueprintReadOnly)
	float YawWalkOffset;

	UPROPERTY(BlueprintReadOnly)
	float YawAimOffset;

	UPROPERTY(EditDefaultsOnly)
	float MaxYawAimOffset;

	UPROPERTY(BlueprintReadOnly)
	FName RightHandSocketName;

	UPROPERTY(BlueprintReadOnly)
	FName LeftHandSocketName;

	UPROPERTY(BlueprintReadOnly)
	FName SecondWeaponSocketName;

	UPROPERTY()
	float EquipWeaponAnimDelay = -0.8f;

private:
	AST_BaseSoldierCharacter* SoldierCharacter;
	UST_BaseWeaponsManagerComponent* WeaponManagerComponent;
	
	AST_BaseWeapon* CurrentWeapon;
	AST_BaseWeapon* AdditionalWeapon;
	AST_BaseWeapon* SwitchingWeapon;
	
	UStaticMeshComponent* CurrentMagazineComponent;
	FTransform CurrentMagazineTransform;

	float CurrentTurnDuration;
	float CurrentTurnAngle;
	float StartingTurnYawAngle;
	ECharacterTurnSide TurningSide;

	bool bIsReloading;
	bool bIsWeaponSwitching;

	FTimerHandle ReloadingTimerHandle;

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	bool IsWeaponSwitching() const { return bIsWeaponSwitching; };
	float GetEqiupWeaponMontageLength() const;

private:
	void UpdateMovingAnimation();
	void UpdateIdleAnimation();
	void UpdateTurningAnimation(float DeltaTime);
	void UpdateLeftHandWeaponPosition();

	FORCEINLINE void OnMovementTypeChanged(ECharacterMovingType InMovingType) { MovingType = InMovingType; };

	void SetupCurrentWeapon();

	void OnWeaponEquipped(int32 WeaponIndex, AST_BaseWeapon* Weapon);
	void OnWeaponUnequipped(int32 WeaponIndex, AST_BaseWeapon* Weapon);
	void OnWeaponFired(AST_BaseWeapon* Weapon);
    void OnWeaponReloading(int32 WeaponIndex, AST_BaseWeapon* Weapon);
	void OnWeaponReloadingInterrupted(AST_BaseWeapon* Weapon);
	void OnWeaponSwitchStarted(int32 PreviousWeaponIndex, int32 NewWeaponIndex);

	void OnWeaponEquippingAnimationFinished();
	
	void OnDamageDealed(float CurrentHealthValue, EHealthChangingType HealthChangingType);

	/**
	 * Animation notifies handlers 
	 */
	UFUNCTION()
	void InternalAnimNotify_OnMagazineGrabbed();
	
	UFUNCTION()
	void InternalAnimNotify_OnMagazineInserted();

	UFUNCTION()
	void InternalAnimNotify_OnWeaponSwitched();
};
