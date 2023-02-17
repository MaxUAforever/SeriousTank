#include "Actors/Pawns/MainMenu/ST_ViewCameraPawn.h"

#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"

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

	PlayerInputComponent->BindAxis(TEXT("RotateMainMenuCamera"), this, &ThisClass::RotateCameraByXAxis);
	PlayerInputComponent->BindAxis(TEXT("RotateCameraByYAxis"), this, &ThisClass::RotateCameraByYAxis);
}

void AST_ViewCameraPawn::RotateCameraByXAxis(float Value)
{
	if (Controller != nullptr)
	{
		RootComponent->AddRelativeRotation(FRotator{ 0, Value, 0 });
	}
}

void AST_ViewCameraPawn::RotateCameraByYAxis(float Value)
{
	if (Controller != nullptr)
	{
		const FRotator CameraRotation = CameraSceneComponent->GetRelativeRotation();
		const float NewPitchRotation = FMath::Clamp(CameraRotation.Pitch + Value, MinPitchCameraAngle, MaxPitchCameraAngle);

		CameraSceneComponent->SetRelativeRotation(FRotator{ NewPitchRotation, CameraRotation.Yaw, CameraRotation.Roll });
	}
}

