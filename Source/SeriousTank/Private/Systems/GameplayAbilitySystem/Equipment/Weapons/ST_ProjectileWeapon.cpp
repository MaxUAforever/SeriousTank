#include "Systems/GameplayAbilitySystem/Equipment/Weapons/ST_ProjectileWeapon.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameplayAbilitySpecHandle.h"
#include "Engine/World.h"
#include "Sound/SoundCue.h"
#include "Systems/GameplayAbilitySystem/Abilities/ST_GunFireGameplayAbility.h"

AST_ProjectileWeapon::AST_ProjectileWeapon()
{
	AimSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("AimSpringArmComponent"));
	AimSpringArm->SetupAttachment(RootComponent);

	AimMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AimMeshComponent"));
	AimMesh->SetupAttachment(AimSpringArm);
	AimMesh->SetOnlyOwnerSee(true);
	AimMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	EquipmentAbilitySetToGrant.GameplayAbilityInfo.GameplayAbilityClass = UST_GunFireGameplayAbility::StaticClass();
	EquipmentAbilitySetToGrant.GameplayAbilityInfo.TargetingType = EST_TargetingType::Auto;
}

void AST_ProjectileWeapon::BeginPlay()
{
	Super::BeginPlay();

	CurrentAmmoCount = TotalAmmoCount;
}

bool AST_ProjectileWeapon::CheckCost()
{
	return CurrentAmmoCount > 0;
}

void AST_ProjectileWeapon::ApplyCost()
{
	--CurrentAmmoCount;
}

void AST_ProjectileWeapon::HandleAbilityActivated(const FGameplayAbilitySpecHandle InHandle)
{
	AimMesh->SetVisibility(true);
}

void AST_ProjectileWeapon::HandleAbilityEnded(UGameplayAbility* InAbility)
{
	AimMesh->SetVisibility(false);
}