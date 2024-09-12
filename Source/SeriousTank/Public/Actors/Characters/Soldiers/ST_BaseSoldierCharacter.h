#pragma once

#include "GameFramework/Character.h"
#include "ST_BaseSoldierCharacter.generated.h"

class AST_BaseWeapon;
class UCameraComponent;
class UCommonInputsDataAsset;
class UInteractingComponent;
class USoldierInputsDataAsset;
class UST_HealthComponent;
class UST_SoldierWeaponManagerComponent;
class UWeaponInputsDataAsset;
struct FInputActionValue;

UCLASS()
class SERIOUSTANK_API AST_BaseSoldierCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	USceneComponent* CameraSceneComponent;

	UPROPERTY(EditAnywhere)
	UCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly)
	UST_SoldierWeaponManagerComponent* WeaponManagerComponent;

	UPROPERTY(EditDefaultsOnly)
	UST_HealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly)
	UInteractingComponent* InteractingComponent;

	/**
	 * Common gameplay and weapon inputs
	 */

	UPROPERTY(Category = "Input",  EditDefaultsOnly)
	UCommonInputsDataAsset* CommonInputsDataAsset;

	UPROPERTY(Category = "Input",  EditDefaultsOnly)
	UWeaponInputsDataAsset* WeaponInputsDataAsset;

	UPROPERTY(Category = "Input",  EditDefaultsOnly)
	USoldierInputsDataAsset* SoldierInputsDataAsset;

	/**
	 * Skeleton information
	 */

	UPROPERTY(Category = "Skeleton", EditDefaultsOnly)
	FName RightHandSocketName;

	UPROPERTY(Category = "Skeleton", EditDefaultsOnly)
	FName LeftHandSocketName;

	UPROPERTY(Category = "Skeleton", EditDefaultsOnly)
	FName SecondWeaponSocketName;

protected:
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

public:	
	AST_BaseSoldierCharacter(const class FObjectInitializer& ObjectInitializer);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void NotifyControllerChanged() override;

	float GetCameraYawAngle() const;

	bool CanUseWeapon() const;

	FName GetRightHandSocketName() const { return RightHandSocketName; };
	FName GetLeftHandSocketName() const { return LeftHandSocketName; };
	FName GetSecondWeaponSocketName() const { return SecondWeaponSocketName; };
	
protected:
	void MoveForward(const FInputActionValue& ActionValue);
	void MoveRight(const FInputActionValue& ActionValue);
	
	void StartSprint();
	void StopSprint();

	void RotateCamera(const FInputActionValue& ActionValue);

	void OnWeaponEquipped(int32 WeaponIndex, AST_BaseWeapon* Weapon);
	void OnWeaponEquippedFinished();

	virtual void StartFire();
	virtual void StopFire();
    virtual void Reload();
    
	virtual void SwitchToFirstWeapon();
	virtual void SwitchToSecondWeapon();
	virtual void SwitchToThirdWeapon();

private:
	void MoveByAxis(const FInputActionValue& ActionValue, EAxis::Type Axis);

	void OnDamageDealed(float CurrentHealthValue);
};
