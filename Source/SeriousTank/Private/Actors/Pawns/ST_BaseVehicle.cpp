#include "Actors/Pawns/ST_BaseVehicle.h"

#include "GameFramework/Gameplay/ST_GameplayGameState.h"

#include "Components/InputComponent.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void AST_BaseVehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* PlayerEnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MoveForwardInputAction)
		{
			PlayerEnhancedInputComponent->BindAction(MoveForwardInputAction, ETriggerEvent::Triggered, this, &ThisClass::MoveForward);
		}

		if (MoveRightInputAction)
		{
			PlayerEnhancedInputComponent->BindAction(MoveRightInputAction, ETriggerEvent::Triggered, this, &ThisClass::MoveRight);
		}

		if (RotateCameraInputAction)
		{
			PlayerEnhancedInputComponent->BindAction(RotateCameraInputAction, ETriggerEvent::Triggered, this, &ThisClass::RotateCamera);
		}

		if (FireInputAction)
		{
			PlayerEnhancedInputComponent->BindAction(FireInputAction, ETriggerEvent::Started, this, &ThisClass::StartFire);
			PlayerEnhancedInputComponent->BindAction(FireInputAction, ETriggerEvent::Completed, this, &ThisClass::StopFire);
		}

		if (SwitchToFirstWeaponInputAction)
		{
			PlayerEnhancedInputComponent->BindAction(SwitchToFirstWeaponInputAction, ETriggerEvent::Started, this, &ThisClass::SwitchToFirstWeapon);
		}

		if (SwitchToSecondWeaponInputAction)
		{
			PlayerEnhancedInputComponent->BindAction(SwitchToSecondWeaponInputAction, ETriggerEvent::Started, this, &ThisClass::SwitchToSecondWeapon);
		}

		if (SwitchToThirdWeaponInputAction)
		{
			PlayerEnhancedInputComponent->BindAction(SwitchToThirdWeaponInputAction, ETriggerEvent::Started, this, &ThisClass::SwitchToThirdWeapon);
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

	if (bIsUnPossessed)
	{
		EnhancedSubsystem->RemoveMappingContext(WeaponsInputContext);
	}
	else
	{
		if (CommonGameplayInputContext && !EnhancedSubsystem->HasMappingContext(CommonGameplayInputContext))
		{
			EnhancedSubsystem->AddMappingContext(CommonGameplayInputContext, 0);
		}
		
		if (WeaponsInputContext && !EnhancedSubsystem->HasMappingContext(WeaponsInputContext))
		{
			EnhancedSubsystem->AddMappingContext(WeaponsInputContext, 0);
		}
	}
}