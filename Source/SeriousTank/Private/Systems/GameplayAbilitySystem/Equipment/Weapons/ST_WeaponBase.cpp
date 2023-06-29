#include "Systems/GameplayAbilitySystem/Equipment/Weapons/ST_WeaponBase.h"

#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"

AST_WeaponBase::AST_WeaponBase()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
	SetRootComponent(SceneComponent);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	MeshComponent->SetupAttachment(RootComponent);

	ShootingArrowComponent = CreateDefaultSubobject<UArrowComponent>("ShootingArrowComponent");
	ShootingArrowComponent->SetupAttachment(RootComponent);

	DefaultReloadingTime = 5.f;
	TotalAmmoCount = 50;
}

const FTransform& AST_WeaponBase::GetMuzzleTransform() const
{
	return ShootingArrowComponent->GetComponentTransform();
}