#include "Actors/Pawns/MainMenu/ST_ViewCameraPawn.h"

#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"

AST_ViewCameraPawn::AST_ViewCameraPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	
	MinPitchCameraAngle = -80.f;
	MaxPitchCameraAngle = 0.f;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
	RootComponent = RootSceneComponent;

	CameraSceneComponent = CreateDefaultSubobject<USceneComponent>("CameraSceneComponent");
	CameraSceneComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(CameraSceneComponent);
}

void AST_ViewCameraPawn::BeginPlay()
{
	Super::BeginPlay();
}

void AST_ViewCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* PlayerEnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (RotateCameraByXAxisInputAction)
		{
			PlayerEnhancedInputComponent->BindAction(RotateCameraByXAxisInputAction, ETriggerEvent::Triggered, this, &ThisClass::RotateCameraByXAxis);
		}

		if (RotateCameraByYAxisInputAction)
		{
			PlayerEnhancedInputComponent->BindAction(RotateCameraByYAxisInputAction, ETriggerEvent::Triggered, this, &ThisClass::RotateCameraByYAxis);
		}
	}
}

void AST_ViewCameraPawn::NotifyControllerChanged()
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
		EnhancedSubsystem->RemoveMappingContext(ViewCameraInputContext);
	}
	else
	{
		EnhancedSubsystem->AddMappingContext(ViewCameraInputContext, 0);
	}
}

void AST_ViewCameraPawn::RotateCameraByXAxis(const FInputActionValue& ActionValue)
{
	if (Controller != nullptr)
	{
		RootComponent->AddRelativeRotation(FRotator{ 0, ActionValue.Get<FInputActionValue::Axis1D>(), 0 });
	}
}

void AST_ViewCameraPawn::RotateCameraByYAxis(const FInputActionValue& ActionValue)
{
	if (Controller != nullptr)
	{
		const FRotator CameraRotation = CameraSceneComponent->GetRelativeRotation();
		const float NewPitchRotation = FMath::Clamp(CameraRotation.Pitch + ActionValue.Get<FInputActionValue::Axis1D>(), MinPitchCameraAngle, MaxPitchCameraAngle);

		CameraSceneComponent->SetRelativeRotation(FRotator{ NewPitchRotation, CameraRotation.Yaw, CameraRotation.Roll });
	}
}

