#pragma once

#include "Animation/AnimInstance.h"
#include "ST_BaseSoldierAnimInstance.generated.h"

class AST_BaseSoldierCharacter;
class AST_BaseWeapon;
enum class ECharacterMovingType : uint8;
enum class ECharacterTurnSide : uint8;

UCLASS()
class SERIOUSTANK_API UST_BaseSoldierAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* TurnLeftMontage;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* TurnRightMontage;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* TwoHandsWeaponFireMontage;

	UPROPERTY(BlueprintReadOnly)
	ECharacterMovingType MovingType;

	UPROPERTY(BlueprintReadOnly)
	bool bIsWeaponEquipped;

	UPROPERTY(BlueprintReadOnly)
	float YawWalkOffset;

	UPROPERTY(BlueprintReadOnly)
	float YawAimOffset;

	UPROPERTY(EditDefaultsOnly)
	float MaxYawAimOffset;

private:
	AST_BaseSoldierCharacter* SoldierCharacter;

	float CurrentTurnDuration;
	float CurrentTurnAngle;
	float StartingTurnYawAngle;
	ECharacterTurnSide TurningSide;

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

private:
	void UpdateMovingAnimation();
	void UpdateIdleAnimation();
	void UpdateTurningAnimation(float DeltaTime);

	FORCEINLINE void OnMovementTypeChanged(ECharacterMovingType InMovingType) { MovingType = InMovingType; };

	void OnWeaponEquipped(int32 WeaponIndex, AST_BaseWeapon* Weapon);
	void OnWeaponFired(AST_BaseWeapon* Weapon);
};
