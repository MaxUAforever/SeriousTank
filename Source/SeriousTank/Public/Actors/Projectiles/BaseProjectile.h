#pragma once

#include "GameFramework/Actor.h"
#include "BaseProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class SERIOUSTANK_API ABaseProjectile : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere)
	USphereComponent* CollisionComponent;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere)
	UProjectileMovementComponent* ProjectileMovementComponent;

public:	
	ABaseProjectile();

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	virtual void BeginPlay() override;
};
