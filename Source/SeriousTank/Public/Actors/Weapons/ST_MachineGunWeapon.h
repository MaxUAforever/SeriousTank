#pragma once

#include "Actors/Weapons/ST_FireArmWeapon.h"
#include "ST_MachineGunWeapon.generated.h"

UCLASS()
class SERIOUSTANK_API AST_MachineGunWeapon : public AST_FireArmWeapon
{
	GENERATED_BODY()
	
public:
    DECLARE_MULTICAST_DELEGATE_OneParam(FOnClipAmmoCountChanged, int32)
    FOnClipAmmoCountChanged OnClipAmmoCountChanged;
    
protected:
	UPROPERTY(EditAnywhere, Category = "Ammo")
	int32 MaxClipAmmoCount;

	UPROPERTY(EditAnywhere, Category = "Ammo")
	float BurstShootingPause;

	UPROPERTY(VisibleAnywhere, Category = "TimerHandler")
	FTimerHandle BurstShootingTimerHandler;

public:
    int32 GetCurrentClipAmmoCount() const { return CurrentClipAmmoCount; };
    
private:
	bool bIsInBurstPause;
	int32 CurrentClipAmmoCount;

public:
	virtual bool IsReloadingNeeded() const override;

protected:
    virtual void BeginPlay() override;
    
	virtual void StartShooting() override;

	virtual bool CanReload() const override;
	virtual void FinishReloading() override;
	virtual void Shoot() override;

	void OnBurstPauseEnded();
};
