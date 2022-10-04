#pragma once

#include "GameFramework/Pawn.h"
#include "ST_BaseVehicle.generated.h"

UCLASS(Abstract)
class SERIOUSTANK_API AST_BaseVehicle : public APawn
{
	GENERATED_BODY()

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void MoveForward(float Value) {};
	virtual void MoveRight(float Value) {};
	virtual void RotateCamera(float Value) {};

	virtual void StartFire() {};
	virtual void StopFire() {};

	virtual void SwitchToFirstWeapon() {};
	virtual void SwitchToSecondWeapon() {};
	virtual void SwitchToThirdWeapon() {};
};
