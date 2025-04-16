#include "Actors/Weapons/ST_FireArmWeapon.h"

#include "Actors/Pawns/ST_BaseVehicle.h"
#include "Actors/Projectiles/ST_BaseProjectile.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "ObjectPoolSubsystem/Public/ObjectPoolSubsystem.h"
#include "Sound/SoundCue.h"
#include "TimerManager.h"

AST_FireArmWeapon::AST_FireArmWeapon()
{
    AimSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("AimSpringArmComponent"));
    AimSpringArm->SetupAttachment(RootComponent);
    
    AimMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AimMeshComponent"));
    AimMesh->SetupAttachment(AimSpringArm);
    AimMesh->SetOnlyOwnerSee(true);
    AimMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AST_FireArmWeapon::AttachToParentComponent(USceneComponent* InParentComponent, FName SocketName, bool bInShouldBeDestroyedWithActor)
{
    if (!InParentComponent)
    {
        return;
    }
    
    Super::AttachToParentComponent(InParentComponent, SocketName, bInShouldBeDestroyedWithActor);
    
    if (AST_BaseVehicle* Vehicle = Cast<AST_BaseVehicle>(InParentComponent->GetOwner()))
    {
        FVector ParentLocation = InParentComponent->GetRelativeLocation();
        AimSpringArm->TargetArmLength = Vehicle->GetMaxVisibleDistance() - AimLocationOffset - ParentLocation.X;
    }
}

void AST_FireArmWeapon::OnSetWeaponEnabled(bool bInIsEnabled)
{
	AimMesh->SetVisibility(bInIsEnabled);
}

void AST_FireArmWeapon::Shoot()
{
	if (!CanShoot())
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return;
	}

	UObjectPoolSubsystem* ObjectPoolSubsystem = World->GetSubsystem<UObjectPoolSubsystem>();
	if (!IsValid(ObjectPoolSubsystem))
	{
		return;
	}

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;

	FTransform SpawnTransform = ShootingArrowComponent->GetComponentTransform();
	SpawnTransform.SetScale3D(GetActorScale3D());

	AST_BaseProjectile* Projectile = ObjectPoolSubsystem->SpawnActor<AST_BaseProjectile>(ProjectileClass, SpawnTransform, SpawnParameters);
	if (!IsValid(Projectile))
	{
		return;
	}
	
	FVector Velocity = ShootingArrowComponent->GetForwardVector();
	Velocity.Z = 0.f;
	Projectile->ResetVelocity(Velocity);
	
	if (ShootSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(World, ShootSound, ShootingArrowComponent->GetComponentLocation());
	}

	SetTotalAmmoCount(GetTotalAmmoCount() - 1);
	OnShootDone.ExecuteIfBound();
}
