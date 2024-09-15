#pragma once

#include "GameFramework/Actor.h"
#include "ST_BaseProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class USoundCue;
class UST_DamageDealingComponent;

UCLASS()
class SERIOUSTANK_API AST_BaseProjectile : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly)
	USphereComponent* CollisionComponent;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly)
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere)
	USoundCue* ExplosionSound;

	UPROPERTY(EditDefaultsOnly)
	UST_DamageDealingComponent* DamageDealingComponent;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AActor>> ActorsToIgnore;

	UPROPERTY(EditDefaultsOnly)
	bool bDrawDebug = false;

public:	
	AST_BaseProjectile();

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	virtual void BeginPlay() override;
};
