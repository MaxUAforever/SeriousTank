#pragma once

#include "Actors/Weapons/ST_FireArmWeapon.h"
#include "MachineGunWeapon.generated.h"

UCLASS()
class SERIOUSTANK_API AMachineGunWeapon : public AST_FireArmWeapon
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "Ammo")
	int32 MaxClipAmmoCount;

	UPROPERTY(EditAnywhere, Category = "Ammo")
	float BurstShootingPause;

	UPROPERTY(VisibleAnywhere, Category = "TimerHandler")
	FTimerHandle BurstShootingTimerHandler;

private:
	bool bIsInBurstPause;
	int32 CurrentClipAmmoCount;

public:
	AMachineGunWeapon();

protected:
	virtual void StartShooting() override;

	virtual void FinishReloading() override;
	virtual void Shoot() override;

	void OnBurstPauseEnded();
};
