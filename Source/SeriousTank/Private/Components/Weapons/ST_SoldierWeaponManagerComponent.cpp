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
#include "Engine/World.h"

namespace 
{
	
TSubclassOf<AST_BaseWeapon> GetPlayerSoldierMainWeaponClass(const APlayerController* PlayerController, const AActor* OwnerActor)
{
	TSubclassOf<AST_BaseWeapon> Result;
	if (!PlayerController)
	{
		return Result;
	}

	if (const AST_MainMenuPlayerState* MenuPlayerState = PlayerController->GetPlayerState<AST_MainMenuPlayerState>())
	{
		Result = MenuPlayerState->GetSoldierInfo().MainWeaponClass;
	}
	else if (const AST_GameplayPlayerState* GameplayPlayerState = PlayerController->GetPlayerState<AST_GameplayPlayerState>())
	{
		if (PlayerController->GetPawn() == OwnerActor)
		{
			Result = GameplayPlayerState->GetSoldierInfo().MainWeaponClass;
		}
	}

	return Result;
}

TSubclassOf<AST_BaseWeapon> GetPlayerSoldierSecondWeaponClass(const APlayerController* PlayerController, const AActor* OwnerActor)
{
	TSubclassOf<AST_BaseWeapon> Result;
	if (!PlayerController)
	{
		return Result;
	}

	if (const AST_MainMenuPlayerState* MenuPlayerState = PlayerController->GetPlayerState<AST_MainMenuPlayerState>())
	{
		Result = MenuPlayerState->GetSoldierInfo().SecondWeaponClass;
	}
	else if (const AST_GameplayPlayerState* GameplayPlayerState = PlayerController->GetPlayerState<AST_GameplayPlayerState>())
	{
		if (PlayerController->GetPawn() == OwnerActor)
		{
			Result = GameplayPlayerState->GetSoldierInfo().SecondWeaponClass;
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

UST_SoldierWeaponManagerComponent::UST_SoldierWeaponManagerComponent()
{
	SetCanChangeWeapons(true);
}

void UST_SoldierWeaponManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	APawn* Pawn = Cast<APawn>(GetOwner());
	if (!Pawn)
	{
		return;
	}

	if (!Cast<APlayerController>(Pawn->GetController()))
	{
		return;
	}

	TSubclassOf<AST_BaseWeapon> CustomWeaponClass = GetPlayerSoldierMainWeaponClass(World->GetFirstPlayerController(), GetOwner());
	if (CustomWeaponClass)
	{
		if (AST_BaseWeapon* Weapon = World->SpawnActor<AST_BaseWeapon>(CustomWeaponClass))
		{
            AddWeapon(Weapon);
		}
	}

	TSubclassOf<AST_BaseWeapon> SecondWeaponClass = GetPlayerSoldierSecondWeaponClass(World->GetFirstPlayerController(), GetOwner());
	if (SecondWeaponClass)
	{
		if (AST_BaseWeapon* Weapon = World->SpawnActor<AST_BaseWeapon>(SecondWeaponClass))
		{
			AddWeapon(Weapon);
		}
	}
}

void UST_SoldierWeaponManagerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	for (AST_BaseWeapon* Weapon : GetWeapons())
	{
		Weapon->OnShootDone.Unbind();
	}

	Super::EndPlay(EndPlayReason);
}

void UST_SoldierWeaponManagerComponent::OnWeaponAdded(AST_BaseWeapon* NewWeapon)
{
    if (!IsValid(NewWeapon))
    {
        return;
    }
    
    NewWeapon->OnShootDone.BindUObject(this, &ThisClass::OnWeaponFired);

	if (USkeletalMeshComponent* CharacterSkeletalMesh = GetOwnerSkeletalMesh(GetOwner()))
	{
		switch (GetWeapons().Num())
		{ 
			case 1:
				NewWeapon->AttachToParentComponent(CharacterSkeletalMesh, RightHandSocketName);
				break;
			case 2:
				NewWeapon->AttachToParentComponent(CharacterSkeletalMesh, SecondWeaponSocketName);
				break;
			default:
				NewWeapon->SetHidden(true);
		}
	}
}

void UST_SoldierWeaponManagerComponent::OnPreWeaponRemoved(int32 WeaponIndex)
{
	AST_BaseWeapon* RemovedWeapon = GetWeapon(WeaponIndex);
	if (!IsValid(RemovedWeapon))
	{
		return;
	}

	if (WeaponIndex == GetCurrentWeaponIndex())
	{
		InterruptReloading();
	}

	RemovedWeapon->OnShootDone.Unbind();
	if (USkeletalMeshComponent* CharacterSkeletalMesh = GetOwnerSkeletalMesh(GetOwner()))
	{
		RemovedWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}

	RemovedWeapon->SetHidden(false);
}

void UST_SoldierWeaponManagerComponent::OnWeaponSwitchingStarted(const int32 PrevWeaponIndex, const int32 NewWeaponIndex)
{
	InterruptReloading();
}

void UST_SoldierWeaponManagerComponent::SetupSockets(const FName InRightHandSocketName, const FName InLeftHandSocketName, const FName InSecondWeaponSocketName)
{
	RightHandSocketName = InRightHandSocketName;
	LeftHandSocketName = InLeftHandSocketName;
	SecondWeaponSocketName = InSecondWeaponSocketName;
}

void UST_SoldierWeaponManagerComponent::OnWeaponFired()
{
	OnWeaponFiredDelegate.ExecuteIfBound(GetCurrentWeapon());
}


