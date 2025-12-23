#pragma once

#include "GameFramework/Pawn.h"
#include "ST_BaseVehicle.generated.h"

class UBoxComponent;
class UCommonInputsDataAsset;
class UInteractionComponent;
class UVehicleInputsDataAsset;
class UWeaponInputsDataAsset;
class UInputAction;
class UInputMappingContext;
class UST_HealthComponent;
class UST_HealthBarWidgetComponent;
struct FInputActionValue;

UCLASS(Abstract)
class SERIOUSTANK_API AST_BaseVehicle : public APawn
{
	GENERATED_BODY()

public:
	AST_BaseVehicle();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void NotifyControllerChanged() override;
	virtual void UnPossessed() override;

    FString GetDisplayName() const { return DisplayName; };
    float GetMaxVisibleDistance() const { return MaxVisibleDistance; };
    
	virtual void MoveForward(const FInputActionValue& ActionValue) {};
	virtual void MoveRight(const FInputActionValue& ActionValue) {};
	virtual void RotateCamera(const FInputActionValue& ActionValue) {};

	virtual void StartFire() {};
	virtual void StopFire() {};

    virtual void SwitchToFirstWeapon() { };
    virtual void SwitchToSecondWeapon() { };
    virtual void SwitchToThirdWeapon() { };

	void ExitVehicle();

private:
	void DisableVehicle();

protected:
	UPROPERTY(EditAnywhere)
	UBoxComponent* BaseCollisionComponent;

	UPROPERTY(EditAnywhere)
	UInteractionComponent* InteractionComponent;

	UPROPERTY(EditDefaultsOnly)
	UST_HealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly)
	UST_HealthBarWidgetComponent* HealthBarWidgetComponent;

	/**
	 * Gameplay information
	 */

	UPROPERTY(EditDefaultsOnly, Category = "Info")
	FString DisplayName;

	UPROPERTY(EditAnywhere, Category = "Camera");
	float MaxVisibleDistance;

	/**
	 * Common gameplay and weapon inputs
	 */

	UPROPERTY(Category = "Input", EditDefaultsOnly)
	UCommonInputsDataAsset* CommonInputsDataAsset;

	UPROPERTY(Category = "Input", EditDefaultsOnly)
	UVehicleInputsDataAsset* VehicleInputsDataAsset;

	UPROPERTY(Category = "Input", EditDefaultsOnly)
	UWeaponInputsDataAsset* WeaponInputsDataAsset;
};
