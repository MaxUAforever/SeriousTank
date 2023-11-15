#pragma once

#include "GameFramework/Pawn.h"
#include "ST_ViewCameraPawn.generated.h"

class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

UCLASS()
class SERIOUSTANK_API AST_ViewCameraPawn : public APawn
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	USceneComponent* RootSceneComponent;

	UPROPERTY(EditAnywhere)
	USceneComponent* CameraSceneComponent;

	UPROPERTY(EditAnywhere)
	UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere)
	float MinPitchCameraAngle;

	UPROPERTY(EditAnywhere)
	float MaxPitchCameraAngle;

	UPROPERTY(Category = "Input", EditDefaultsOnly)
	UInputMappingContext* ViewCameraInputContext;

	UPROPERTY(Category = "Input",  EditDefaultsOnly)
    UInputAction* RotateCameraByXAxisInputAction;

	UPROPERTY(Category = "Input",  EditDefaultsOnly)
    UInputAction* RotateCameraByYAxisInputAction;

private:
	FRotator AddedRotation;

public:
	AST_ViewCameraPawn();

protected:
	virtual void BeginPlay() override;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void NotifyControllerChanged() override;

private:
	void RotateCameraByXAxis(const FInputActionValue& ActionValue);
	void RotateCameraByYAxis(const FInputActionValue& ActionValue);
};
