#pragma once

#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

class UArrowComponent;

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

public:
	ABaseWeapon();

	virtual void StartFire() {};
	virtual void StopFire() {};
};
