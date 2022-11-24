#include "Components/ST_WeaponsManagerComponent.h"

#include "Components/ST_WeaponSocketComponent.h"
#include "Core/ST_CoreTypes.h"
#include "GameFramework/Gameplay/ST_GameplayPlayerState.h"
#include "GameFramework/MainMenu/ST_MainMenuPlayerState.h"
#include "GameFramework/ST_GameInstance.h"

#include "GameFramework/Actor.h"


UST_WeaponsManagerComponent::UST_WeaponsManagerComponent()
{
	CurrentWeaponIndex = 0;
}

void UST_WeaponsManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}
    
    APlayerController* PlayerController = World->GetFirstPlayerController();
    if (!PlayerController)
    {
        return;
    }
    
    TArray<TSubclassOf<ABaseWeapon>> CustomWeaponClasses;
    if (AST_MainMenuPlayerState* MenuPlayerState = PlayerController->GetPlayerState<AST_MainMenuPlayerState>())
    {
        CustomWeaponClasses = MenuPlayerState->GetCurrentVehicle().WeaponClasses;
    }
    else if (AST_GameplayPlayerState* GameplayPlayerState = PlayerController->GetPlayerState<AST_GameplayPlayerState>())
    {
        CustomWeaponClasses = GameplayPlayerState->GetVehicleInfo().WeaponClasses;
    }
    
	TArray<UActorComponent*> WeaponSocketActors;
	GetOwner()->GetComponents(UST_WeaponSocketComponent::StaticClass(), WeaponSocketActors);
	Weapons.Reserve(WeaponSocketActors.Num());
    
	int32 WeaponIndex = 0;
	for (UActorComponent* WeaponSocketActor : WeaponSocketActors)
	{
		UST_WeaponSocketComponent* WeaponSocket = Cast<UST_WeaponSocketComponent>(WeaponSocketActor);
        
        bool bHasCustomWeapon = WeaponIndex < CustomWeaponClasses.Num() && CustomWeaponClasses[WeaponIndex] != nullptr;
        TSubclassOf<ABaseWeapon> CurrentWeaponClass = bHasCustomWeapon ? CustomWeaponClasses[WeaponIndex] : WeaponSocket->GetDefaultWeaponClass();
		
		ABaseWeapon* Weapon = CurrentWeaponClass ? WeaponSocket->SetWeapon(CurrentWeaponClass) : nullptr;
        if (Weapon)
        {
            Weapon->SetWeaponEnabled(WeaponIndex == 0);
        }
        
        Weapons.Add(Weapon);
        WeaponIndex++;
	}
}

void UST_WeaponsManagerComponent::StartFire()
{
	if (Weapons.Num() != 0)
	{
		Weapons[CurrentWeaponIndex]->StartFire();
	}
}

void UST_WeaponsManagerComponent::StopFire()
{
	if (Weapons.Num() != 0)
	{
		Weapons[CurrentWeaponIndex]->StopFire();
	}
}

bool UST_WeaponsManagerComponent::SwitchWeapon(int32 WeaponIndex)
{
	if (WeaponIndex < 0 || WeaponIndex >= Weapons.Num() || Weapons[WeaponIndex] == nullptr)
	{
		return false;
	}

    StopFire();
	
    Weapons[CurrentWeaponIndex]->SetWeaponEnabled(false);
    Weapons[WeaponIndex]->SetWeaponEnabled(true);
    
    CurrentWeaponIndex = WeaponIndex;
	
	return true;
}

ABaseWeapon* UST_WeaponsManagerComponent::GetWeapon(int32 WeaponIndex) const
{
	return (WeaponIndex >= 0 && WeaponIndex < Weapons.Num()) ? Weapons[CurrentWeaponIndex] : nullptr;
}
