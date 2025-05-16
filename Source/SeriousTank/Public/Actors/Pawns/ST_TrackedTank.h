#pragma once

#include "Actors/Pawns/ST_BaseTrackedVehicle.h"
#include "AI/Interfaces/ST_AIPawnInterface.h"
#include "ST_TrackedTank.generated.h"

class UArrowComponent;
class UAudioComponent;
class USoundCue;
class UST_WeaponSocketComponent;
class UST_VehicleWeaponManagerComponent;

UCLASS()
class SERIOUSTANK_API AST_TrackedTank : public AST_BaseTrackedVehicle, public IST_AIPawnInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	USceneComponent* TurretSceneComponent;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* TurretMeshComponent;

	UPROPERTY(EditAnywhere)
	UST_WeaponSocketComponent* MainWeaponSocketComponent;

	UPROPERTY(EditAnywhere)
	UST_WeaponSocketComponent* SecondWeaponSocketComponent;

	UPROPERTY(EditAnywhere)
	UAudioComponent* TurretRotationSound;

	UPROPERTY(EditDefaultsOnly)
	UST_VehicleWeaponManagerComponent* WeaponManagerComponent;

protected:
	UPROPERTY(EditAnywhere)
	float TurretRotationSpeed;

	UPROPERTY(EditAnywhere, Category = "Aiming|Debug")
	bool bDrawDebugAimingTarget;

	UPROPERTY(EditAnywhere, Category = "Aiming|Debug")
	float DrawHeightOffset;

public:
	AST_TrackedTank();

public:
	// IST_AIPawnInterface - Begin
	virtual void AimToLocation(const FVector& Location) override;
	virtual bool IsAiming() const override;
	// IST_AIPawnInterface - End

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void NotifyControllerChanged() override;

private:
	void RotateTurretToCamera(float DeltaTime);

	virtual void StartFire() override;
	virtual void StopFire() override;

	virtual void SwitchToFirstWeapon() override;
	virtual void SwitchToSecondWeapon() override;
};
