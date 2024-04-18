#include "Actors/Projectiles/ST_BaseProjectile.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Subsystems/HealthSubsystem/ST_DamageDealingComponent.h"

AST_BaseProjectile::AST_BaseProjectile()
{
	CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionComponent");
	SetRootComponent(CollisionComponent);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	MeshComponent->SetupAttachment(RootComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
	DamageDealingComponent = CreateDefaultSubobject<UST_DamageDealingComponent>("DamageDealingComponent");
}

void AST_BaseProjectile::BeginPlay()
{
	Super::BeginPlay();

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);
}

void AST_BaseProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }
    
    APlayerController* PC = World->GetFirstPlayerController();
    if (!PC)
    {
        return;
    }
    
	if (OtherActor == this || OtherActor == GetOwner() || OtherActor == PC->GetPawn())
	{
		return;
	}

	// Prevent errors in case of spawning target on projectile location.
	SetActorEnableCollision(false);
	
	DamageDealingComponent->StartDealingDamage(OtherActor, SweepResult.ImpactPoint);

	if (ExplosionSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(World, ExplosionSound, GetActorLocation());
	}

	Destroy();
}

