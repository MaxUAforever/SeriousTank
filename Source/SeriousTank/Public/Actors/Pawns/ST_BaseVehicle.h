#pragma once

#include "GameFramework/Pawn.h"
#include "ST_BaseVehicle.generated.h"

class UCommonInputsDataAsset;
class UWeaponInputsDataAsset;
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
	 * Common gameplay and weapon inputs
	 */

	UPROPERTY(Category = "Input",  EditDefaultsOnly)
	UCommonInputsDataAsset* CommonInputsDataAsset;

	UPROPERTY(Category = "Input",  EditDefaultsOnly)
	UWeaponInputsDataAsset* WeaponInputsDataAsset;

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
