#include "Actors/Projectiles/ST_BaseProjectile.h"

#include "Actors/GameplayActors/ST_ShootTarget.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AST_BaseProjectile::AST_BaseProjectile()
{
	CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionComponent");
	SetRootComponent(CollisionComponent);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	MeshComponent->SetupAttachment(RootComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
}

void AST_BaseProjectile::BeginPlay()
{
	Super::BeginPlay();

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);
}

void AST_BaseProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AST_ShootTarget* ShootingTargetActor = Cast<AST_ShootTarget>(OtherActor))
	{
		OtherActor->Destroy();
	}
	if (OtherActor != this && OtherActor != GetOwner())
	{
		Destroy();
	}
}

