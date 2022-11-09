#include "Actors/GameplayActors/ST_ShootTarget.h"

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

