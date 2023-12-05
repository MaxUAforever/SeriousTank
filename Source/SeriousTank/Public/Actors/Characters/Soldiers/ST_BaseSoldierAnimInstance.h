#pragma once

#include "Animation/AnimInstance.h"
#include "ST_BaseSoldierAnimInstance.generated.h"

class AST_BaseSoldierCharacter;
class AST_BaseWeapon;
enum class ECharacterMovingType : uint8;

DECLARE_DELEGATE_OneParam(FOnMaxAimAngleReached, float);

UCLASS()
class SERIOUSTANK_API UST_BaseSoldierAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	FOnMaxAimAngleReached OnMaxAimAngleReached;

protected:
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

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

private:
	FORCEINLINE void OnMovementTypeChanged(ECharacterMovingType InMovingType) { MovingType = InMovingType; };

	void OnWeaponEquipped(int32 WeaponIndex, AST_BaseWeapon* Weapon);
};
