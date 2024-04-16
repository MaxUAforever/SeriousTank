#pragma once

#include "Core/Animation/ST_AnimInstance.h"
#include "ST_BaseSoldierAnimInstance.generated.h"

class AST_BaseSoldierCharacter;
class AST_BaseWeapon;
enum class ECharacterMovingType : uint8;
enum class ECharacterTurnSide : uint8;

UCLASS()
class SERIOUSTANK_API UST_BaseSoldierAnimInstance : public UST_AnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* TurnLeftMontage;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* TurnRightMontage;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* TwoHandsWeaponFireMontage;

    UPROPERTY(EditDefaultsOnly)
    UAnimMontage* TwoHandsWeaponReloadingMontage;
    
	UPROPERTY(BlueprintReadOnly)
	ECharacterMovingType MovingType;

	UPROPERTY(BlueprintReadOnly)
	bool bIsWeaponEquipped;
    
    UPROPERTY(BlueprintReadOnly)
    bool bIsWeaponFiring;

	UPROPERTY(BlueprintReadOnly)
	float YawWalkOffset;

	UPROPERTY(BlueprintReadOnly)
	float YawAimOffset;

	UPROPERTY(EditDefaultsOnly)
	float MaxYawAimOffset;

	UPROPERTY(BlueprintReadOnly)
	FTransform LeftHandTransform;

	UPROPERTY(EditDefaultsOnly)
	FName RightHandBoneName;

	UPROPERTY(EditDefaultsOnly)
	FName RightHandSocketName;

	UPROPERTY(EditDefaultsOnly)
	FName LeftHandSocketName;

private:
	AST_BaseSoldierCharacter* SoldierCharacter;
	AST_BaseWeapon* CurrentWeapon;
	UStaticMeshComponent* CurrentMagazineComponent;
	FTransform CurrentMagazineTransform;

	float CurrentTurnDuration;
	float CurrentTurnAngle;
	float StartingTurnYawAngle;
	ECharacterTurnSide TurningSide;

	bool bIsReloading;

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

private:
	void UpdateMovingAnimation();
	void UpdateIdleAnimation();
	void UpdateTurningAnimation(float DeltaTime);
	void UpdateLeftHandWeaponPosition();

	FORCEINLINE void OnMovementTypeChanged(ECharacterMovingType InMovingType) { MovingType = InMovingType; };

	void OnWeaponEquipped(int32 WeaponIndex, AST_BaseWeapon* Weapon);
	void OnWeaponFired(AST_BaseWeapon* Weapon);
    void OnWeaponReloading(AST_BaseWeapon* Weapon);

	UFUNCTION()
	void InternalAnimNotify_OnMagazineGrabbed();

	UFUNCTION()
	void InternalAnimNotify_OnMagazineInserted();
};
