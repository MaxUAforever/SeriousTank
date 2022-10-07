#include "Actors/Projectiles/BaseProjectile.h"

#include "Actors/GameplayActors/ST_ShootTarget.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ABaseProjectile::ABaseProjectile()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
	SetRootComponent(SceneComponent);

	CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionComponent");
	CollisionComponent->SetupAttachment(SceneComponent);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	MeshComponent->SetupAttachment(SceneComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
}

void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);
}

void ABaseProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AST_ShootTarget* ShootingTargetActor = Cast<AST_ShootTarget>(OtherActor))
	{
		OtherActor->Destroy();
	}
}