#include "Actors/Weapons/BaseWeapon.h"

#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ABaseWeapon::ABaseWeapon()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
	SetRootComponent(SceneComponent);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	MeshComponent->SetupAttachment(SceneComponent);

	ShootingArrowComponent = CreateDefaultSubobject<UArrowComponent>("ShootingArrowComponent");
	ShootingArrowComponent->SetupAttachment(SceneComponent);
}

