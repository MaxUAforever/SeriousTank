#pragma once

#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

class UArrowComponent;
class USoundCue;

UCLASS(Abstract)
class SERIOUSTANK_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere)
	UArrowComponent* ShootingArrowComponent;

	UPROPERTY(EditDefaultsOnly)
	USoundCue* ShootSound;

	UPROPERTY(VisibleAnywhere, Category = "TimerHandler")
	FTimerHandle ReloadTimerHandler;

public:
	ABaseWeapon();

	virtual void StartFire() {};
	virtual void StopFire() {};

	virtual bool IsReloading() const { return false; };
	virtual float GetReloadingRemainingTime() const { return 0.f; };

	DECLARE_DELEGATE(FReloadingStarted)
	FReloadingStarted OnReloadingStarted;
};
