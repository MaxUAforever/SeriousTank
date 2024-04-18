#include "Actors/GameplayActors/ST_ShootTarget.h"

#include "Actors/Projectiles/ST_BaseProjectile.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Gameplay/ST_GameplayGameMode.h"

AST_ShootTarget::AST_ShootTarget()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
	SetRootComponent(SceneComponent);

	CollisionCompnent = CreateDefaultSubobject<UBoxComponent>("CollisionComponent");
	CollisionCompnent->SetupAttachment(SceneComponent);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	MeshComponent->SetupAttachment(SceneComponent);
}

void AST_ShootTarget::BeginPlay()
{
	Super::BeginPlay();

	CollisionCompnent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);
}

void AST_ShootTarget::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<AST_BaseProjectile>(OtherActor))
	{
		Destroy();
	}
}

