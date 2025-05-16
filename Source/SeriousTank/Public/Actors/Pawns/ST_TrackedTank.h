#pragma once

#include "Actors/Pawns/ST_BaseTrackedVehicle.h"
#include "ST_TrackedTank.generated.h"

class UArrowComponent;
class UAudioComponent;
class USoundCue;
class UST_WeaponSocketComponent;
class UST_VehicleWeaponManagerComponent;

UCLASS()
class SERIOUSTANK_API AST_TrackedTank : public AST_BaseTrackedVehicle
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

public:
	AST_TrackedTank();

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
