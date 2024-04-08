#include "Actors/Pawns/ST_BaseVehicle.h"

#include "GameFramework/Gameplay/ST_GameplayGameState.h"
#include "Inputs/Data/CommonInputsDataAsset.h"
#include "Inputs/Data/WeaponInputsDataAsset.h"
#include "Components/InputComponent.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void AST_BaseVehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* PlayerEnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (CommonInputsDataAsset)
		{
			PlayerEnhancedInputComponent->BindAction(CommonInputsDataAsset->MoveForwardInputAction, ETriggerEvent::Triggered, this, &ThisClass::MoveForward);
			PlayerEnhancedInputComponent->BindAction(CommonInputsDataAsset->MoveRightInputAction, ETriggerEvent::Triggered, this, &ThisClass::MoveRight);
			PlayerEnhancedInputComponent->BindAction(CommonInputsDataAsset->RotateCameraInputAction, ETriggerEvent::Triggered, this, &ThisClass::RotateCamera);
		}

		if (WeaponInputsDataAsset)
		{
			PlayerEnhancedInputComponent->BindAction(WeaponInputsDataAsset->FireInputAction, ETriggerEvent::Started, this, &ThisClass::StartFire);
			PlayerEnhancedInputComponent->BindAction(WeaponInputsDataAsset->FireInputAction, ETriggerEvent::Completed, this, &ThisClass::StopFire);
		
			PlayerEnhancedInputComponent->BindAction(WeaponInputsDataAsset->SwitchToFirstWeaponInputAction, ETriggerEvent::Started, this, &ThisClass::SwitchToFirstWeapon);
			PlayerEnhancedInputComponent->BindAction(WeaponInputsDataAsset->SwitchToSecondWeaponInputAction, ETriggerEvent::Started, this, &ThisClass::SwitchToSecondWeapon);
			PlayerEnhancedInputComponent->BindAction(WeaponInputsDataAsset->SwitchToThirdWeaponInputAction, ETriggerEvent::Started, this, &ThisClass::SwitchToThirdWeapon);
		}
	}
}

void AST_BaseVehicle::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	const bool bIsUnPossessed = Controller == nullptr;

	APlayerController* PC = Cast<APlayerController>(bIsUnPossessed ? PreviousController : Controller);
	if (!PC)
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* EnhancedSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
	if (!EnhancedSubsystem)
	{
		return;
	}

	if (bIsUnPossessed && WeaponInputsDataAsset)
	{
		EnhancedSubsystem->RemoveMappingContext(WeaponInputsDataAsset->WeaponsInputContext);
	}
	else
	{
		if (CommonInputsDataAsset && !EnhancedSubsystem->HasMappingContext(CommonInputsDataAsset->CommonGameplayInputContext))
		{
			EnhancedSubsystem->AddMappingContext(CommonInputsDataAsset->CommonGameplayInputContext, 0);
		}
		
		if (WeaponInputsDataAsset && !EnhancedSubsystem->HasMappingContext(WeaponInputsDataAsset->WeaponsInputContext))
		{
			EnhancedSubsystem->AddMappingContext(WeaponInputsDataAsset->WeaponsInputContext, 0);
		}
	}
}