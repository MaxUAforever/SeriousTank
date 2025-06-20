#include "Actors/Projectiles/ST_BaseProjectile.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h" 
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "ObjectPoolSubsystem/Public/ObjectPoolSubsystem.h"
#include "Sound/SoundCue.h"
#include "Subsystems/HealthSubsystem/ST_DamageDealingComponent.h"
#include "TimerManager.h"

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

	ActorsToIgnore.Add(AST_BaseProjectile::StaticClass());
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);
}

void AST_BaseProjectile::ResetVelocity(FVector Direction)
{
	if (IsValid(ProjectileMovementComponent) && ProjectileMovementComponent->InitialSpeed > 0.f)
	{
		ProjectileMovementComponent->Velocity = Direction.GetSafeNormal() * ProjectileMovementComponent->InitialSpeed;
	}
}

void AST_BaseProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    UWorld* World = GetWorld();
    if (!World)
    {
		UE_LOG(LogTemp, Warning, TEXT("ST_BaseProjectile::OnBeginOverlap: World is not valid for projectile"));
        return;
    }
    
	if (OtherActor == this || OtherActor == GetOwner() || ActorsToIgnore.Contains(OtherActor->GetClass()))
	{
		return;
	}
	
	AController* OwningController = Cast<AController>(GetOwner());
	if (!IsValid(OwningController))
	{
		UE_LOG(LogTemp, Warning, TEXT("OwningController is not valid for projectile %s"), *GetName());
		return;
	}

	UObjectPoolSubsystem* ObjectPoolSubsystem = World->GetSubsystem<UObjectPoolSubsystem>();
	if (IsValid(ObjectPoolSubsystem))
	{
		ObjectPoolSubsystem->ReturnToPool(this);
	}

	DamageDealingComponent->StartDealingDamage(OwningController, OtherActor, SweepResult.ImpactPoint);

	if (ExplosionSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(World, ExplosionSound, GetActorLocation());
	}

	if (bDrawDebug)
	{
		DrawDebugSphere(GetWorld(), GetActorLocation(), 3.f, 12, FColor::Red, false, 3.f, 0U, 3.f);
	}
}

