#pragma once

#include "GameFramework/Pawn.h"
#include "ST_BaseVehicle.generated.h"

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
    
public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    FString GetDisplayName() const { return DisplayName; };
    float GetMaxVisibleDistance() const { return MaxVisibleDistance; };
    
	virtual void MoveForward(float Value) {};
	virtual void MoveRight(float Value) {};
	virtual void RotateCamera(float Value) {};

	virtual void StartFire() {};
	virtual void StopFire() {};

    virtual void SwitchToFirstWeapon() { OnWeaponSwitched.Broadcast(0); };
    virtual void SwitchToSecondWeapon() { OnWeaponSwitched.Broadcast(1); };
    virtual void SwitchToThirdWeapon() { OnWeaponSwitched.Broadcast(2); };
};
