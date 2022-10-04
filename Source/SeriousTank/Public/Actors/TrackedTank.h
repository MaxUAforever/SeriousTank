#pragma once

#include "Actors/BaseTrackedVehicle.h"
#include "TrackedTank.generated.h"

class UArrowComponent;
class UST_WeaponSocketComponent;
class UST_WeaponsManagerComponent;

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
	UST_WeaponSocketComponent* MainWeaponSocketComponent;

	UPROPERTY(EditAnywhere)
	UST_WeaponSocketComponent* SecondWeaponSocketComponent;

	UPROPERTY(VisibleAnywhere)
	UST_WeaponsManagerComponent* WeaponsManagerComponent;

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

	template<int32 WeaponIndex>
	void SwitchWeapon()
	{
		WeaponsManagerComponent->SwitchWeapon(WeaponIndex);
	}
};
