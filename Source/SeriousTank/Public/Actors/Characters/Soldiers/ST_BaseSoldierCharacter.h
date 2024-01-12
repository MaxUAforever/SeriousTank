#pragma once

#include "GameFramework/Character.h"
#include "ST_BaseSoldierCharacter.generated.h"

class AST_BaseWeapon;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class UST_SoldierWeaponManagerComponent;
struct FInputActionValue;

UCLASS()
class SERIOUSTANK_API AST_BaseSoldierCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnWeaponSwitched, int32)
	FOnWeaponSwitched OnWeaponSwitched;

protected:
	UPROPERTY(EditAnywhere)
	USceneComponent* CameraSceneComponent;

	UPROPERTY(EditAnywhere)
	UCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly)
	UST_SoldierWeaponManagerComponent* WeaponManagerComponent;

	/**
	 * Common gameplay inputs
	 */

	UPROPERTY(Category = "Input",  EditDefaultsOnly)
    UInputMappingContext* CommonGameplayInputContext;

	UPROPERTY(Category = "Input",  EditDefaultsOnly)
    UInputAction* MoveForwardInputAction;

	UPROPERTY(Category = "Input",  EditDefaultsOnly)
    UInputAction* MoveRightInputAction;

	UPROPERTY(Category = "Input",  EditDefaultsOnly)
    UInputAction* RotateCameraInputAction;

	/**
	 * Soldier weapons inputs
	 */

	UPROPERTY(Category = "Input",  EditDefaultsOnly)
    UInputMappingContext* WeaponsInputContext;

	UPROPERTY(Category = "Input",  EditDefaultsOnly)
    UInputAction* FireInputAction;

	UPROPERTY(Category = "Input", EditDefaultsOnly)
	UInputAction* SwitchToFirstWeaponInputAction;

	UPROPERTY(Category = "Input", EditDefaultsOnly)
	UInputAction* SwitchToSecondWeaponInputAction;

	UPROPERTY(Category = "Input", EditDefaultsOnly)
	UInputAction* SwitchToThirdWeaponInputAction;

	/**
	 * Character gameplay inputs
	 */

	UPROPERTY(Category = "Input", EditDefaultsOnly)
	UInputMappingContext* SoldierGameplayInputContext;

	UPROPERTY(Category = "Input", EditDefaultsOnly)
	UInputAction* SprintInputAction;

protected:
	virtual void BeginPlay() override;

public:	
	AST_BaseSoldierCharacter(const class FObjectInitializer& ObjectInitializer);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void NotifyControllerChanged() override;

	float GetCameraYawAngle() const;

protected:
	void MoveForward(const FInputActionValue& ActionValue);
	void MoveRight(const FInputActionValue& ActionValue);
	
	void StartSprint();
	void StopSprint();

	void RotateCamera(const FInputActionValue& ActionValue);

	void OnWeaponEquipped(int32 WeaponIndex, AST_BaseWeapon* Weapon);

	virtual void StartFire();
	virtual void StopFire();

	virtual void SwitchToFirstWeapon();
	virtual void SwitchToSecondWeapon();
	virtual void SwitchToThirdWeapon();

private:
	void MoveByAxis(const FInputActionValue& ActionValue, EAxis::Type Axis);
};
