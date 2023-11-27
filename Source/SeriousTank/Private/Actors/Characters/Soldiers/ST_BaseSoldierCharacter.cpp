#include "Actors/Characters/Soldiers/ST_BaseSoldierCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/ST_SoldierMovementComponent.h"
#include "Components/Weapons/ST_SoldierWeaponManagerComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
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
		if (MoveForwardInputAction)
		{
			EnhancedComponent->BindAction(MoveForwardInputAction, ETriggerEvent::Triggered, this, &ThisClass::MoveForward);
		}

		if (MoveRightInputAction)
		{
			EnhancedComponent->BindAction(MoveRightInputAction, ETriggerEvent::Triggered, this, &ThisClass::MoveRight);
		}

		if (RotateCameraInputAction)
		{
			EnhancedComponent->BindAction(RotateCameraInputAction, ETriggerEvent::Triggered, this, &ThisClass::RotateCamera);
		}

		if (SprintInputAction)
		{
			EnhancedComponent->BindAction(SprintInputAction, ETriggerEvent::Started, this, &ThisClass::StartSprint);
			EnhancedComponent->BindAction(SprintInputAction, ETriggerEvent::Completed, this, &ThisClass::StopSprint);
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
		if (SoldierGameplayInputContext)
		{
			EnhancedSubsystem->RemoveMappingContext(SoldierGameplayInputContext);
		}
	}
	else
	{
		if (CommonGameplayInputContext && !EnhancedSubsystem->HasMappingContext(CommonGameplayInputContext))
		{
			EnhancedSubsystem->AddMappingContext(CommonGameplayInputContext, 0);
		}

		if (SoldierGameplayInputContext && !EnhancedSubsystem->HasMappingContext(SoldierGameplayInputContext))
		{
			EnhancedSubsystem->AddMappingContext(SoldierGameplayInputContext, 0);
		}
	}
}

float AST_BaseSoldierCharacter::GetCameraYawAngle() const
{
	return CameraSceneComponent->GetComponentRotation().Yaw;
}

void AST_BaseSoldierCharacter::MoveForward(const FInputActionValue& ActionValue)
{
	FInputActionValue::Axis1D Value = ActionValue.Get<FInputActionValue::Axis1D>();

	if (Controller && Value != 0)
	{
		if (!GetCharacterMovement()->bOrientRotationToMovement)
		{
			SetActorRotation(CameraSceneComponent->GetComponentRotation());
		}

		AddMovementInput(FRotationMatrix(CameraSceneComponent->GetComponentRotation()).GetScaledAxis(EAxis::X), Value);
	}
}

void AST_BaseSoldierCharacter::MoveRight(const FInputActionValue& ActionValue)
{
	FInputActionValue::Axis1D Value = ActionValue.Get<FInputActionValue::Axis1D>();

	if (Controller && Value != 0)
	{
		AddMovementInput(FRotationMatrix(CameraSceneComponent->GetComponentRotation()).GetScaledAxis(EAxis::Y), Value);
	}
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
