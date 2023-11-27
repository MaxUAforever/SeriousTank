#include "Components/Weapons/ST_SoldierWeaponManagerComponent.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/ST_WeaponSocketComponent.h"
#include "Core/ST_CoreTypes.h"
#include "GameFramework/Gameplay/ST_GameplayPlayerState.h"
#include "GameFramework/MainMenu/ST_MainMenuPlayerState.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/ST_GameInstance.h"
#include "GameFramework/Actor.h"
#include "Actors/Weapons/ST_BaseWeapon.h"

namespace 
{
	
TSubclassOf<AST_BaseWeapon> GetPlayerSoldierWeaponClass(const APlayerController* PlayerController, const AActor* OwnerActor)
{
	TSubclassOf<AST_BaseWeapon> Result;
	if (!PlayerController)
	{
		return Result;
	}

	if (const AST_MainMenuPlayerState* MenuPlayerState = PlayerController->GetPlayerState<AST_MainMenuPlayerState>())
	{
		Result = MenuPlayerState->GetSoldierInfo().WeaponClass;
	}
	else if (const AST_GameplayPlayerState* GameplayPlayerState = PlayerController->GetPlayerState<AST_GameplayPlayerState>())
	{
		if (PlayerController->GetPawn() == OwnerActor)
		{
			Result = GameplayPlayerState->GetSoldierInfo().WeaponClass;
		}
	}

	return Result;
}

USkeletalMeshComponent* GetOwnerSkeletalMesh(const AActor* OwnerActor)
{
	TArray<UActorComponent*> CharacterSkeletalMeshes;
	OwnerActor->GetComponents(USkeletalMeshComponent::StaticClass(), CharacterSkeletalMeshes);

	if (CharacterSkeletalMeshes.Num() > 0)
	{
		return Cast<USkeletalMeshComponent>(CharacterSkeletalMeshes[0]);
	}

	return nullptr;
}

} // anonymous namespace

void UST_SoldierWeaponManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	TSubclassOf<AST_BaseWeapon> CustomWeaponClass = GetPlayerSoldierWeaponClass(World->GetFirstPlayerController(), GetOwner());
	USkeletalMeshComponent* CharacterSkeletalMesh = GetOwnerSkeletalMesh(GetOwner());

	if (CustomWeaponClass && CharacterSkeletalMesh)
	{
		AST_BaseWeapon* Weapon = World->SpawnActor<AST_BaseWeapon>(CustomWeaponClass);
		if (Weapon)
		{
			Weapon->SetActorEnableCollision(false);
			Weapon->AttachToComponent(CharacterSkeletalMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, RightHandSocketName);
			Weapons.Add(Weapon);

			OnWeaponAdded.Broadcast(0, Weapon);
		}
	}
}

void UST_SoldierWeaponManagerComponent::OnWeaponSwitched(const int32 PrevWeaponIndex, const int32 NewWeaponIndex)
{
	if (!Weapons.IsValidIndex(PrevWeaponIndex) || !Weapons[PrevWeaponIndex] || Weapons.IsValidIndex(NewWeaponIndex) || !Weapons[NewWeaponIndex])
	{
		return;
	}

	Weapons[PrevWeaponIndex]->SetHidden(true);
	Weapons[NewWeaponIndex]->SetHidden(false);
}