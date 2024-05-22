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
#include "PlayerInteractionSubsystem/Public/InteractionComponent.h"

AST_BaseVehicle::AST_BaseVehicle()
{
	PrimaryActorTick.bCanEverTick = true;

	BaseCollisionComponent = CreateDefaultSubobject<UBoxComponent>("CollisionComponent");
	SetRootComponent(BaseCollisionComponent);

	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>("InteractionComponent");
	InteractionComponent->SetupAttachment(BaseCollisionComponent);
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
	
	if (VehicleInputsDataAsset && !EnhancedSubsystem->HasMappingContext(VehicleInputsDataAsset->VehicleInputContext))
	{
		EnhancedSubsystem->AddMappingContext(VehicleInputsDataAsset->VehicleInputContext, 0);
	}

	if (WeaponInputsDataAsset && !EnhancedSubsystem->HasMappingContext(WeaponInputsDataAsset->WeaponsInputContext))
	{
		EnhancedSubsystem->AddMappingContext(WeaponInputsDataAsset->WeaponsInputContext, 0);
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

	Super::UnPossessed();
}

void AST_BaseVehicle::ExitVehicle()
{
	auto SetActorLocationIfPossible = [this](AActor* Actor, const float ActorCollisionRadius, const float ActorCollisionHelfheight, const FVector& Location) -> bool
	{
		if (!Actor)
		{
			return false;
		}

		TArray<AActor*> OutActors;
		if (!UKismetSystemLibrary::CapsuleOverlapActors(this, Location, ActorCollisionRadius, ActorCollisionHelfheight, {}, nullptr, {}, OutActors))
		{
			Actor->SetActorLocation(Location, true);
			return true;
		}

		return false;
	};

	auto TrySetLocationForExit = [this, SetActorLocationIfPossible](APawn* SoldierPawn) -> bool
	{
		if (!SoldierPawn)
		{
			return false;
		}

		FVector VehicleLocation = GetActorLocation();

		float SoldierCollisionRadius;
		float SoldierCollisionHalfLength;
		UCapsuleComponent* CapsuleComponent = Cast<UCapsuleComponent>(SoldierPawn->GetRootComponent());
		if (!CapsuleComponent)
		{
			return false;
		}

		CapsuleComponent->GetScaledCapsuleSize(SoldierCollisionRadius, SoldierCollisionHalfLength);

		const FVector RightExitLocation = GetActorLocation() + (GetActorRightVector() * (BaseCollisionComponent->GetScaledBoxExtent().Y + SoldierCollisionRadius + 1.f));
		const FVector LeftExitLocation = GetActorLocation() + (GetActorRightVector() * (-1.f) * (BaseCollisionComponent->GetScaledBoxExtent().Y + SoldierCollisionRadius + 1.f));
		const FVector BackExitLocation = GetActorLocation() + (GetActorForwardVector() * (-1.f) * (BaseCollisionComponent->GetScaledBoxExtent().X + SoldierCollisionRadius + 1.f));
		const FVector ForwardExitLocation = GetActorLocation() + (GetActorForwardVector() * (BaseCollisionComponent->GetScaledBoxExtent().X + SoldierCollisionRadius + 1.f));

		const TArray<FVector>& ExitLocations{ RightExitLocation ,LeftExitLocation , BackExitLocation , ForwardExitLocation };

		for (const FVector ExitLocation : ExitLocations)
		{
			if (SetActorLocationIfPossible(SoldierPawn, SoldierCollisionRadius, SoldierCollisionHalfLength, ExitLocation))
			{
				return true;
			}
		}

		return false;
	};


	if (AST_GameplayPlayerController* PlayerController = Cast<AST_GameplayPlayerController>(GetController()))
	{
		APawn* SoldierPawn = PlayerController->GetPreviousPawn();
		if (!TrySetLocationForExit(SoldierPawn))
		{
			return;
		}

		FInputActionValue::Axis1D ZeroInput = 0.f;
		MoveForward(ZeroInput);
		MoveRight(ZeroInput);
		StopFire();
	
		PlayerController->UnPossess();
	
		SoldierPawn->GetRootComponent()->SetVisibility(true, true);
		SoldierPawn->SetActorEnableCollision(true);
	
		PlayerController->Possess(SoldierPawn);
	}
}
