#pragma once

#include "GameFramework/Pawn.h"
#include "ST_BaseVehicle.generated.h"

class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

UCLASS(Abstract)
class SERIOUSTANK_API AST_BaseVehicle : public APawn
{
	GENERATED_BODY()

public:
    DECLARE_MULTICAST_DELEGATE_OneParam(FOnWeaponSwitched, int32)
    FOnWeaponSwitched OnWeaponSwitched;
    
protected:
    UPROPERTY(EditDefaultsOnly, Category = "Info")
    FString DisplayName;
    
    UPROPERTY(EditAnywhere, Category = "Camera");
    float MaxVisibleDistance;
    
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
	 * Vehicle weapons inputs
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

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void NotifyControllerChanged() override;

    FString GetDisplayName() const { return DisplayName; };
    float GetMaxVisibleDistance() const { return MaxVisibleDistance; };
    
	virtual void MoveForward(const FInputActionValue& ActionValue) {};
	virtual void MoveRight(const FInputActionValue& ActionValue) {};
	virtual void RotateCamera(const FInputActionValue& ActionValue) {};

	virtual void StartFire() {};
	virtual void StopFire() {};

    virtual void SwitchToFirstWeapon() { OnWeaponSwitched.Broadcast(0); };
    virtual void SwitchToSecondWeapon() { OnWeaponSwitched.Broadcast(1); };
    virtual void SwitchToThirdWeapon() { OnWeaponSwitched.Broadcast(2); };
};
