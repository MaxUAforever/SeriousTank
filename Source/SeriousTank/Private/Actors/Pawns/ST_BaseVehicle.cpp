#include "Actors/Pawns/ST_BaseVehicle.h"

#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Gameplay/ST_GameplayGameState.h"
#include "GameFramework/Gameplay/ST_GameplayPlayerController.h"
#include "Inputs/Data/CommonInputsDataAsset.h"
#include "Inputs/Data/VehicleInputsDataAsset.h"
#include "Inputs/Data/WeaponInputsDataAsset.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerInteractionSubsystem/Public/Components/InteractionComponent.h"
#include "PlayerInteractionSubsystem/Public/Components/InteractingComponent.h"
#include "Subsystems/HealthSubsystem/Components/ST_HealthBarWidgetComponent.h"
#include "Subsystems/HealthSubsystem/Components/ST_HealthComponent.h"

AST_BaseVehicle::AST_BaseVehicle()
{
	PrimaryActorTick.bCanEverTick = true;

	BaseCollisionComponent = CreateDefaultSubobject<UBoxComponent>("CollisionComponent");
	SetRootComponent(BaseCollisionComponent);

	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>("InteractionComponent");
	InteractionComponent->SetupAttachment(BaseCollisionComponent);

	HealthBarWidgetComponent = CreateDefaultSubobject<UST_HealthBarWidgetComponent>("HealthBarWidgetComponent");
	HealthBarWidgetComponent->SetupAttachment(RootComponent);
	HealthBarWidgetComponent->SetVisibility(true);

	HealthComponent = CreateDefaultSubobject<UST_HealthComponent>("HealthComponent");
}

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

		if (VehicleInputsDataAsset)
		{
			PlayerEnhancedInputComponent->BindAction(VehicleInputsDataAsset->ExitVehicleInputAction, ETriggerEvent::Started, this, &ThisClass::ExitVehicle);
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

	APlayerController* PC = Cast<APlayerController>(Controller);
	if (!PC)
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* EnhancedSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
	if (!EnhancedSubsystem)
	{
		return;
	}

	if (CommonInputsDataAsset && !EnhancedSubsystem->HasMappingContext(CommonInputsDataAsset->CommonGameplayInputContext))
	{
		EnhancedSubsystem->AddMappingContext(CommonInputsDataAsset->CommonGameplayInputContext, 0);
	}

	if (WeaponInputsDataAsset && !EnhancedSubsystem->HasMappingContext(WeaponInputsDataAsset->WeaponsInputContext))
	{
		EnhancedSubsystem->AddMappingContext(WeaponInputsDataAsset->WeaponsInputContext, 0);
	}

	if (VehicleInputsDataAsset && !EnhancedSubsystem->HasMappingContext(VehicleInputsDataAsset->VehicleInputContext))
	{
		FModifyContextOptions ModifyContextOptions;
		ModifyContextOptions.bForceImmediately = true;
		EnhancedSubsystem->AddMappingContext(VehicleInputsDataAsset->VehicleInputContext, 0, ModifyContextOptions);
	}
}

void AST_BaseVehicle::UnPossessed()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* EnhancedSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			if (WeaponInputsDataAsset)
			{
				EnhancedSubsystem->RemoveMappingContext(WeaponInputsDataAsset->WeaponsInputContext);
			}

			if (VehicleInputsDataAsset)
			{
				EnhancedSubsystem->RemoveMappingContext(VehicleInputsDataAsset->VehicleInputContext);
			}
		}
	}

	if (IsValid(InputComponent))
	{
		InputComponent->ClearActionBindings();
	}

	DisableVehicle();

	Super::UnPossessed();
}

void AST_BaseVehicle::ExitVehicle()
{
	if (!IsValid(InteractionComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Failed to get valid InteractionComponent."), ANSI_TO_TCHAR(__FUNCTION__));
		return;
	}
	
	InteractionComponent->StopInteraction();
}

void AST_BaseVehicle::DisableVehicle()
{
	FInputActionValue::Axis1D ZeroInput = 0.f;
	MoveForward(ZeroInput);
	MoveRight(ZeroInput);
	StopFire();
}
