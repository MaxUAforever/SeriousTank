#include "Actors/Characters/Soldiers/ST_BaseSoldierCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ST_SoldierMovementComponent.h"
#include "Components/Weapons/ST_SoldierWeaponManagerComponent.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "Inputs/Data/CommonInputsDataAsset.h"
#include "Inputs/Data/SoldierInputsDataAsset.h"
#include "Inputs/Data/WeaponInputsDataAsset.h"
#include "UObject/UObjectGlobals.h"

AST_BaseSoldierCharacter::AST_BaseSoldierCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UST_SoldierMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	CameraSceneComponent = CreateDefaultSubobject<USceneComponent>("CameraSceneComponent");
	CameraSceneComponent->SetupAttachment(RootComponent);
	CameraSceneComponent->SetUsingAbsoluteRotation(true);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(CameraSceneComponent);

	WeaponManagerComponent = CreateDefaultSubobject<UST_SoldierWeaponManagerComponent>("WeaponManagerComponent");
}

void AST_BaseSoldierCharacter::BeginPlay()
{
	Super::BeginPlay();

	WeaponManagerComponent->OnWeaponAdded.AddUObject(this, &ThisClass::OnWeaponEquipped);
	if (AST_BaseWeapon* Weapon = WeaponManagerComponent->GetCurrentWeapon())
	{
		OnWeaponEquipped(WeaponManagerComponent->GetCurrentWeaponIndex(), Weapon);
	}
}

void AST_BaseSoldierCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (CommonInputsDataAsset)
		{
			EnhancedComponent->BindAction(CommonInputsDataAsset->MoveForwardInputAction, ETriggerEvent::Triggered, this, &ThisClass::MoveForward);
			EnhancedComponent->BindAction(CommonInputsDataAsset->MoveRightInputAction, ETriggerEvent::Triggered, this, &ThisClass::MoveRight);
			EnhancedComponent->BindAction(CommonInputsDataAsset->RotateCameraInputAction, ETriggerEvent::Triggered, this, &ThisClass::RotateCamera);
		}

		if (WeaponInputsDataAsset)
		{
			EnhancedComponent->BindAction(WeaponInputsDataAsset->FireInputAction, ETriggerEvent::Started, this, &ThisClass::StartFire);
			EnhancedComponent->BindAction(WeaponInputsDataAsset->FireInputAction, ETriggerEvent::Completed, this, &ThisClass::StopFire);
			EnhancedComponent->BindAction(WeaponInputsDataAsset->ReloadInputAction, ETriggerEvent::Started, this, &ThisClass::Reload);
			
			EnhancedComponent->BindAction(WeaponInputsDataAsset->SwitchToFirstWeaponInputAction, ETriggerEvent::Started, this, &ThisClass::SwitchToFirstWeapon);
			EnhancedComponent->BindAction(WeaponInputsDataAsset->SwitchToSecondWeaponInputAction, ETriggerEvent::Started, this, &ThisClass::SwitchToSecondWeapon);
			EnhancedComponent->BindAction(WeaponInputsDataAsset->SwitchToThirdWeaponInputAction, ETriggerEvent::Started, this, &ThisClass::SwitchToThirdWeapon);
		}

		if (SoldierInputsDataAsset)
		{
			EnhancedComponent->BindAction(SoldierInputsDataAsset->SprintInputAction, ETriggerEvent::Started, this, &ThisClass::StartSprint);
			EnhancedComponent->BindAction(SoldierInputsDataAsset->SprintInputAction, ETriggerEvent::Completed, this, &ThisClass::StopSprint);
		}
	}
}

void AST_BaseSoldierCharacter::NotifyControllerChanged()
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
		if (SoldierInputsDataAsset)
		{
			EnhancedSubsystem->RemoveMappingContext(SoldierInputsDataAsset->SoldierGameplayInputContext);
		}

		if (WeaponInputsDataAsset)
		{
			EnhancedSubsystem->RemoveMappingContext(WeaponInputsDataAsset->WeaponsInputContext);
		}
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

		if (SoldierInputsDataAsset && !EnhancedSubsystem->HasMappingContext(SoldierInputsDataAsset->SoldierGameplayInputContext))
		{
			EnhancedSubsystem->AddMappingContext(SoldierInputsDataAsset->SoldierGameplayInputContext, 0);
		}
	}
}

float AST_BaseSoldierCharacter::GetCameraYawAngle() const
{
	return CameraSceneComponent->GetComponentRotation().Yaw;
}

void AST_BaseSoldierCharacter::MoveForward(const FInputActionValue& ActionValue)
{
	MoveByAxis(ActionValue, EAxis::X);
}

void AST_BaseSoldierCharacter::MoveRight(const FInputActionValue& ActionValue)
{
	MoveByAxis(ActionValue, EAxis::Y);
}

void AST_BaseSoldierCharacter::StartSprint()
{
	if (UST_SoldierMovementComponent* SoldierMovementComponent = Cast<UST_SoldierMovementComponent>(GetMovementComponent()))
	{
		SoldierMovementComponent->SetSprintActive(true);
	}
}

void AST_BaseSoldierCharacter::StopSprint()
{
	if (UST_SoldierMovementComponent* SoldierMovementComponent = Cast<UST_SoldierMovementComponent>(GetMovementComponent()))
	{
		SoldierMovementComponent->SetSprintActive(false);
	}
}

void AST_BaseSoldierCharacter::RotateCamera(const FInputActionValue& ActionValue)
{
	FInputActionValue::Axis1D Value = ActionValue.Get<FInputActionValue::Axis1D>();

	if (!Controller || FMath::IsNearlyZero(Value))
	{
		return;
	}
		
	CameraSceneComponent->AddLocalRotation(FRotator{ 0, Value, 0 });

	// In case of character moving it is needed to rotate him in direction of camera.
	if (!GetCharacterMovement()->bOrientRotationToMovement)
	{
		if (GetMovementComponent()->Velocity.SizeSquared2D() > 0)
		{
			SetActorRotation(CameraSceneComponent->GetComponentRotation());
		}
	}
}

void AST_BaseSoldierCharacter::OnWeaponEquipped(int32 WeaponIndex, AST_BaseWeapon* Weapon)
{
	GetCharacterMovement()->bOrientRotationToMovement = false;
	
	SetActorRotation(CameraSceneComponent->GetComponentRotation());
	//CameraSceneComponent->SetUsingAbsoluteRotation(false);
}

void AST_BaseSoldierCharacter::StartFire()
{
	WeaponManagerComponent->StartFire();
}

void AST_BaseSoldierCharacter::StopFire()
{
	WeaponManagerComponent->StopFire();
}

void AST_BaseSoldierCharacter::Reload()
{
    WeaponManagerComponent->Reload();
}

void AST_BaseSoldierCharacter::SwitchToFirstWeapon()
{
	WeaponManagerComponent->SwitchWeapon(0);
}

void AST_BaseSoldierCharacter::SwitchToSecondWeapon()
{
	WeaponManagerComponent->SwitchWeapon(1);
}

void AST_BaseSoldierCharacter::SwitchToThirdWeapon()
{
	WeaponManagerComponent->SwitchWeapon(2);
}

void AST_BaseSoldierCharacter::MoveByAxis(const FInputActionValue& ActionValue, EAxis::Type Axis)
{
	FInputActionValue::Axis1D Value = ActionValue.Get<FInputActionValue::Axis1D>();

	if (Controller && Value != 0)
	{
		if (!GetCharacterMovement()->bOrientRotationToMovement)
		{
			SetActorRotation(CameraSceneComponent->GetComponentRotation());
		}

		AddMovementInput(FRotationMatrix(CameraSceneComponent->GetComponentRotation()).GetScaledAxis(Axis), Value);
	}
}
