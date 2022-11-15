#include "Actors/Weapons/ST_FireArmWeapon.h"

#include "Actors/Projectiles/ST_BaseProjectile.h"

#include "Components/ArrowComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "TimerManager.h"

void AST_FireArmWeapon::Shoot()
{
	if (IsReloading())
	{
		return;
	}

	if (UWorld* World = GetWorld())
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = this;

		World->SpawnActor<AST_BaseProjectile>(ProjectileClass, ShootingArrowComponent->GetComponentTransform(), SpawnParameters);

		if (ShootSound)
		{
			UGameplayStatics::SpawnSoundAtLocation(World, ShootSound, ShootingArrowComponent->GetComponentLocation());
		}
	}
}

void AST_FireArmWeapon::FinishReloading()
{
	// Firearm weapons have to shoot immediately after reloading if fire is forced.
	if (IsFireForced())
	{
		StartShooting();
	}
}