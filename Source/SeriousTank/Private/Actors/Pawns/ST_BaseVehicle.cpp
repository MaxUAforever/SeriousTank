#include "Actors/Pawns/ST_BaseVehicle.h"

#include "Abilities/GameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "Components/InputComponent.h"
#include "Components/ST_WeaponSocketComponent.h"
#include "Components/ST_WeaponSocketManagerComponent.h"
#include "Core/ST_CoreTypes.h"
#include "Engine/World.h"
#include "GameFramework/Gameplay/ST_GameplayGameState.h"
#include "GameFramework/Gameplay/ST_GameplayPlayerState.h"
#include "GameFramework/MainMenu/ST_MainMenuPlayerState.h"
#include "Systems/GameplayAbilitySystem/ST_EquipmentManagerComponent.h"
#include "Systems/GameplayAbilitySystem/ST_VehicleAbilitySystemComponent.h"

AST_BaseVehicle::AST_BaseVehicle()
{
	AbilitySystemComponent = CreateDefaultSubobject<UST_VehicleAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	WeaponSocketManagerComponent = CreateDefaultSubobject<UST_WeaponSocketManagerComponent>(TEXT("WeaponSocketManagerComponent"));
	EquipmentManagerComponent = CreateDefaultSubobject<UST_EquipmentManagerComponent>("EquipmentManagerComponent");
}

void AST_BaseVehicle::BeginPlay()
{
	Super::BeginPlay();

	SetupEquipment();
}

void AST_BaseVehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	FTopLevelAssetPath AbilityEnumAssetPath = FTopLevelAssetPath(FName("/Script/SeriousTank"), FName("EST_AbilityInputID"));
	FGameplayAbilityInputBinds AbilityInput(FString("ConfirmAbility"), FString("CancelAbility"), AbilityEnumAssetPath, static_cast<int32>(EST_AbilityInputID::ConfirmAbility), static_cast<int32>(EST_AbilityInputID::CancelAbility));

	AbilitySystemComponent->BindAbilityActivationToInputComponent(PlayerInputComponent, AbilityInput);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ThisClass::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ThisClass::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("RotateCamera"), this, &ThisClass::RotateCamera);
}

void AST_BaseVehicle::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	SetOwner(NewController);
}

void AST_BaseVehicle::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}

void AST_BaseVehicle::SetupEquipment() const
{
	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("ST_BaseVehicle::SetupEquipment: Failed to get World."))
		return;
	}

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("ST_BaseVehicle::SetupEquipment: Failed to get PlayerController."))
		return;
	}

	// TODO: Replace to GameFramework class and create setter in Vehicle class.
	TArray<TSubclassOf<AST_WeaponBase>> CustomWeaponClasses;
	if (AST_MainMenuPlayerState* MenuPlayerState = PlayerController->GetPlayerState<AST_MainMenuPlayerState>())
	{
		CustomWeaponClasses = MenuPlayerState->GetCurrentVehicle().WeaponClasses;
	}
	else if (AST_GameplayPlayerState* GameplayPlayerState = PlayerController->GetPlayerState<AST_GameplayPlayerState>())
	{
		CustomWeaponClasses = GameplayPlayerState->GetVehicleInfo().WeaponClasses;
	}

	for (int32 Index = 0; Index < WeaponSocketManagerComponent->GetWeaponSockets().Num(); ++Index)
	{
		UST_WeaponSocketComponent* WeaponSocket = WeaponSocketManagerComponent->GetWeaponSocket(Index);
		if (!WeaponSocket)
		{
			UE_LOG(LogTemp, Warning, TEXT("ST_BaseVehicle::SetupEquipment: Failed to get WeaponSocket by index %d"), Index);
		}

		bool bHasCustomWeapon = Index < CustomWeaponClasses.Num() && CustomWeaponClasses[Index] != nullptr;
		TSubclassOf<AST_WeaponBase> CurrentWeaponClass = bHasCustomWeapon ? CustomWeaponClasses[Index] : WeaponSocket->GetDefaultWeaponClass();
		
		EquipmentManagerComponent->EquipItemByIndex(CurrentWeaponClass, Index, WeaponSocket);
	}

}