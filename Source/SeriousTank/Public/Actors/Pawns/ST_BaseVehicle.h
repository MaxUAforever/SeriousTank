#pragma once

#include "GameFramework/Pawn.h"
#include "AbilitySystemInterface.h"
#include "ST_BaseVehicle.generated.h"

class UAbilitySystemComponent;
class UGameplayAbility;

UCLASS(Abstract)
class SERIOUSTANK_API AST_BaseVehicle : public APawn, public IAbilitySystemInterface
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

	UPROPERTY()
	UAbilitySystemComponent* AbilitySystemComponent;
    
public:
	AST_BaseVehicle();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void NotifyControllerChanged() override;

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

	/**
	 * Abilities
	 */

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; };

	void AddAbility(TSubclassOf<UGameplayAbility> InAbility);
};
