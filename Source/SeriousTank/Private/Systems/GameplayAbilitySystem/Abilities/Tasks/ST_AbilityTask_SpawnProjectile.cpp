#include "Systems/GameplayAbilitySystem/Abilities/Tasks/ST_AbilityTask_SpawnProjectile.h"

#include "Actors/Projectiles/ST_BaseProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Systems/GameplayAbilitySystem/Equipment/Weapons/ST_ProjectileWeapon.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ST_AbilityTask_SpawnProjectile)

void UST_AbilityTask_SpawnProjectile::Activate()
{
	if (!SourceWeapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnProjectile::Activate: Failed to get SourceWeapon."))
		return;
	}

	if (UWorld* World = GetWorld())
	{ 
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = SourceWeapon;

		AST_BaseProjectile* Projectile = World->SpawnActor<AST_BaseProjectile>(SourceWeapon->GetProjectileClass(), SourceWeapon->GetMuzzleTransform(), SpawnParameters);
		if (!Projectile)
		{
			UE_LOG(LogTemp, Warning, TEXT("SpawnProjectile::Activate: Failed to spawn projectile."));
		}
		else if (SourceWeapon->GetShootSound())
		{
			UGameplayStatics::SpawnSoundAtLocation(World, SourceWeapon->GetShootSound(), SourceWeapon->GetMuzzleTransform().GetLocation());
		}

		OnSpawnProjectile.Broadcast(Projectile);
	}
}

UST_AbilityTask_SpawnProjectile* UST_AbilityTask_SpawnProjectile::SpawnProjectile(UGameplayAbility* OwningAbility)
{
	UST_AbilityTask_SpawnProjectile* TaskObject = NewAbilityTask<UST_AbilityTask_SpawnProjectile>(OwningAbility);
	TaskObject->SourceWeapon = Cast<AST_ProjectileWeapon>(OwningAbility->GetSourceObject(OwningAbility->GetCurrentAbilitySpecHandle(), OwningAbility->GetCurrentActorInfo()));

	return TaskObject;
}


