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
	
	if (CustomWeaponClass)
	{
		if (AST_BaseWeapon* Weapon = World->SpawnActor<AST_BaseWeapon>(CustomWeaponClass))
		{
            AddWeapon(Weapon);
		}
	}
}

void UST_SoldierWeaponManagerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	for (AST_BaseWeapon* Weapon : Weapons)
	{
		Weapon->OnShootDone.Unbind();
	}

	Super::EndPlay(EndPlayReason);
}

void UST_SoldierWeaponManagerComponent::AddWeapon(AST_BaseWeapon* NewWeapon)
{
    if (!NewWeapon)
    {
        return;
    }
    
    Super::AddWeapon(NewWeapon);
    
    NewWeapon->SetActorEnableCollision(false);
    NewWeapon->OnShootDone.BindUObject(this, &ThisClass::OnWeaponFired);
    NewWeapon->OnReloadingStarted.AddUObject(this, &ThisClass::OnWeaponReloadingStarted);
    
    if (USkeletalMeshComponent* CharacterSkeletalMesh = GetOwnerSkeletalMesh(GetOwner()))
    {
        NewWeapon->AttachToComponent(CharacterSkeletalMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, RightHandSocketName);
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

void UST_SoldierWeaponManagerComponent::OnWeaponFired()
{
	OnWeaponFiredDelegate.ExecuteIfBound(GetCurrentWeapon());
}

void UST_SoldierWeaponManagerComponent::OnWeaponReloadingStarted()
{
    OnWeaponReloadingStartedDelegate.Broadcast(GetCurrentWeapon());
}
