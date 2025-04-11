#include "Components/Weapons/ST_VehicleWeaponManagerComponent.h"

#include "Actors/Pawns/ST_BaseVehicle.h"
#include "Components/ST_WeaponSocketComponent.h"
#include "Core/ST_CoreTypes.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Gameplay/ST_GameplayPlayerState.h"
#include "GameFramework/MainMenu/ST_MainMenuPlayerState.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/ST_GameInstance.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

void UST_VehicleWeaponManagerComponent::BeginPlay()
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

	TArray<TSubclassOf<AST_BaseWeapon>> CustomWeaponClasses;
	if (AST_MainMenuPlayerState* MenuPlayerState = PlayerController->GetPlayerState<AST_MainMenuPlayerState>())
	{
		CustomWeaponClasses = MenuPlayerState->GetCurrentVehicle().WeaponClasses;
	}
	else if (AST_GameplayPlayerState* GameplayPlayerState = PlayerController->GetPlayerState<AST_GameplayPlayerState>())
	{
		if (PlayerController->GetPawn() == GetOwner())
		{
			CustomWeaponClasses = GameplayPlayerState->GetVehicleInfo().WeaponClasses;
		}
	}

	TArray<UActorComponent*> WeaponSocketActors;
	GetOwner()->GetComponents(UST_WeaponSocketComponent::StaticClass(), WeaponSocketActors);
	
	int32 WeaponIndex = 0;
	for (UActorComponent* WeaponSocketActor : WeaponSocketActors)
	{
		UST_WeaponSocketComponent* WeaponSocket = Cast<UST_WeaponSocketComponent>(WeaponSocketActor);

		bool bHasCustomWeapon = WeaponIndex < CustomWeaponClasses.Num() && CustomWeaponClasses[WeaponIndex] != nullptr;
		TSubclassOf<AST_BaseWeapon> CurrentWeaponClass = bHasCustomWeapon ? CustomWeaponClasses[WeaponIndex] : WeaponSocket->GetDefaultWeaponClass();

		AST_BaseWeapon* Weapon = CurrentWeaponClass ? WeaponSocket->SetWeapon(CurrentWeaponClass) : nullptr;
		if (Weapon)
		{
			Weapon->SetEnabled(WeaponIndex == 0);
			Weapon->SetActorEnableCollision(false);

            AddWeapon(Weapon);
		}
		
		WeaponIndex++;
	}
}
