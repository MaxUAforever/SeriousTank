#pragma once

#include "GameFramework/Character.h"
#include "ST_BaseSoldierCharacter.generated.h"

class UCameraComponent;
class UInputAction;
class UInputMappingContext;
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

protected:
	void MoveForward(const FInputActionValue& ActionValue);
	void MoveRight(const FInputActionValue& ActionValue);
	
	void StartSprint();
	void StopSprint();

	void RotateCamera(const FInputActionValue& ActionValue);
};
