#include "Actors/GameplayActors/ST_TargetSpawningVolume.h"

#include "Components/BoxComponent.h"

AST_TargetSpawningVolume::AST_TargetSpawningVolume()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
	SetRootComponent(SceneComponent);

	SpawningBoxComponent = CreateDefaultSubobject<UBoxComponent>("SpawningBoxComponent");
	SpawningBoxComponent->SetupAttachment(RootComponent);

	SetActorEnableCollision(false);
}