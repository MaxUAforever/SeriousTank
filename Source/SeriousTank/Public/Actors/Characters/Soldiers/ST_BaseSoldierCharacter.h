#pragma once

#include "GameFramework/Character.h"
#include "ST_BaseSoldierCharacter.generated.h"

class AAIController;
class AST_BaseWeapon;
class UCameraComponent;
class UCommonInputsDataAsset;
class UInteractingComponent;
class USoldierInputsDataAsset;
class UST_HealthComponent;
class UST_SoldierWeaponManagerComponent;
class UST_ViewAreaBoxComponent;
class UWeaponInputsDataAsset;
class UST_HealthBarWidgetComponent;

class UAIPerceptionStimuliSourceComponent;
enum class EHealthChangingType : uint8;
enum class ESoldierActionState : uint8;
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

	UPROPERTY(VisibleAnywhere)
	UST_ViewAreaBoxComponent* CameraViewAreaComponent;

	UPROPERTY(EditDefaultsOnly)
	UST_SoldierWeaponManagerComponent* WeaponManagerComponent;

	UPROPERTY(EditDefaultsOnly)
	UST_HealthBarWidgetComponent* HealthBarWidgetComponent;

	UPROPERTY(EditDefaultsOnly)
	UST_HealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly)
	UInteractingComponent* InteractingComponent;

	UPROPERTY()
	UAIPerceptionStimuliSourceComponent* PerceptionStimuliSourceComponent;

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
	
private:
	ESoldierActionState CurrentActionState;

protected:
	virtual void BeginPlay() override;

public:	
	AST_BaseSoldierCharacter(const class FObjectInitializer& ObjectInitializer);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void NotifyControllerChanged() override;

	float GetCameraYawAngle() const;

	FName GetRightHandSocketName() const { return RightHandSocketName; };
	FName GetLeftHandSocketName() const { return LeftHandSocketName; };
	FName GetSecondWeaponSocketName() const { return SecondWeaponSocketName; };
	
protected:
	void MoveForward(const FInputActionValue& ActionValue);
	void MoveRight(const FInputActionValue& ActionValue);
	
	void StartSprint();
	void StopSprint();

	void RotateCamera(const FInputActionValue& ActionValue);

	void Interact();

	void OnWeaponEquipped(int32 WeaponIndex, AST_BaseWeapon* Weapon);
	void OnWeaponUnequipped(int32 WeaponIndex, AST_BaseWeapon* Weapon);
	void OnWeaponEquippedFinished();
	void OnWeaponReloadingFinished(int32 InWeaponIndex, AST_BaseWeapon* Weapon);

	void StartFire();
	void StopFire();
    void Reload();

	void DropCurrentWeapon();

	void SwitchToFirstWeapon();
	void SwitchToSecondWeapon();
	void SwitchToThirdWeapon();

	bool SwitchToActionState(ESoldierActionState NewActionState);

private:
	void SwitchToWeapon(int32 WeaponIndex);

	void MoveByAxis(const FInputActionValue& ActionValue, EAxis::Type Axis);

	void OnHealthChanged(float CurrentHealthValue, EHealthChangingType HealthChangingType);
	void OnTeamWasChanged(const AController* InController, uint8 TeamId);
	void OnInteractionStopped();

	void OnAIControllerChanged(AAIController* OldAIController, AAIController* NewAIController);
	void OnPlayerControllerChanged(APlayerController* OldPlayerController, APlayerController* NewPlayerController);
};
