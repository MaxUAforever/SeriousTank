#pragma once

#include "Actors/BaseTrackedVehicle.h"

#include "Actors/Weapons/BaseWeapon.h"

#include "TrackedTank.generated.h"

class UArrowComponent;

UCLASS()
class SERIOUSTANK_API ATrackedTank : public ABaseTrackedVehicle
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	USceneComponent* TurretSceneComponent;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* TurretMeshComponent;

	UPROPERTY(EditAnywhere)
	UArrowComponent* MainWeaponArrowComponent;

	UPROPERTY(EditAnywhere)
	UArrowComponent* SecondWeaponArrowComponent;

	UPROPERTY(EditAnywhere, Category="Weapons")
	TSubclassOf<ABaseWeapon> MainWeaponClass;

	UPROPERTY(EditAnywhere, Category = "Weapons")
	TSubclassOf<ABaseWeapon> SecondWeaponClass;

private:
	ABaseWeapon* MainWeapon;
	ABaseWeapon* SecondWeapon;

	ABaseWeapon* CurrentWeapon;

protected:
	UPROPERTY(EditAnywhere)
	float TurretRotationSpeed;

public:
	ATrackedTank();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

private:
	void RotateTurretToCamera(float DeltaTime);

	void StartFire();
	void StopFire();
};
