#include "Actors/Weapons/ST_FireArmWeapon.h"

#include "Actors/Pawns/ST_BaseVehicle.h"
#include "Actors/Projectiles/ST_BaseProjectile.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
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

void AST_FireArmWeapon::SetWeaponEnabled(bool bIsEnabled)
{
    AimMesh->SetVisibility(bIsEnabled);
}

void AST_FireArmWeapon::Shoot()
{
	if (!CanShoot())
	{
		return;
	}

	if (UWorld* World = GetWorld())
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = this;

		FTransform SpawnTransform = ShootingArrowComponent->GetComponentTransform();
		SpawnTransform.SetScale3D(FVector(1.f));
		
		FRotator SpawnRotator = SpawnTransform.Rotator();
		SpawnRotator.Pitch = 0.f;
		SpawnTransform.SetRotation(SpawnRotator.Quaternion());

		World->SpawnActor<AST_BaseProjectile>(ProjectileClass, ShootingArrowComponent->GetComponentTransform(), SpawnParameters);

		if (ShootSound)
		{
			UGameplayStatics::SpawnSoundAtLocation(World, ShootSound, ShootingArrowComponent->GetComponentLocation());
		}
	}

	OnShootDone.ExecuteIfBound();
    OnAmmoCountChanged.Broadcast(--TotalAmmoCount);
}

void AST_FireArmWeapon::FinishReloading()
{
	// Firearm weapons have to shoot immediately after reloading if fire is forced.
	if (IsFireForced())
	{
		StartShooting();
	}
}
