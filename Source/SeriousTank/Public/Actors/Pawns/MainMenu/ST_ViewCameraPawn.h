#pragma once

#include "GameFramework/Pawn.h"
#include "ST_ViewCameraPawn.generated.h"

class UCameraComponent;

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

private:
	FRotator AddedRotation;

public:
	AST_ViewCameraPawn();

protected:
	virtual void BeginPlay() override;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void RotateCameraByXAxis(float Value);
	void RotateCameraByYAxis(float Value);
};
