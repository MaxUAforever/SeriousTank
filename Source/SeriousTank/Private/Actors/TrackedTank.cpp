#include "Actors/TrackedTank.h"

#include "Components/ArrowComponent.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"

ATrackedTank::ATrackedTank()
{
	TurretRotationSpeed = 30.f;

	TurretSceneComponent = CreateDefaultSubobject<USceneComponent>("TurretSceneComponent");
	TurretSceneComponent->SetupAttachment(VehicleSceneComponent);

	TurretMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("TurretMeshComponent");
	TurretMeshComponent->SetupAttachment(TurretSceneComponent);

	MainWeaponArrowComponent = CreateDefaultSubobject<UArrowComponent>("MainWeaponArrowComponent");
	MainWeaponArrowComponent->SetupAttachment(TurretSceneComponent);

	SecondWeaponArrowComponent = CreateDefaultSubobject<UArrowComponent>("ShootingArrowComponent");
	SecondWeaponArrowComponent->SetupAttachment(TurretSceneComponent);
}

void ATrackedTank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("StartFire", EInputEvent::IE_Pressed, this, &ThisClass::StartFire);
	PlayerInputComponent->BindAction("StopFire", EInputEvent::IE_Released, this, &ThisClass::StopFire);
}

void ATrackedTank::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	if (MainWeaponClass)
	{
		MainWeapon = World->SpawnActor<ABaseWeapon>(MainWeaponClass, MainWeaponArrowComponent->GetComponentTransform());
		MainWeapon->AttachToComponent(TurretSceneComponent, FAttachmentTransformRules::KeepWorldTransform);
		CurrentWeapon = MainWeapon;
	}

	if (SecondWeaponClass)
	{
		SecondWeapon = World->SpawnActor<ABaseWeapon>(SecondWeaponClass, SecondWeaponArrowComponent->GetComponentTransform());
		SecondWeapon->AttachToComponent(TurretSceneComponent, FAttachmentTransformRules::KeepWorldTransform);
	}
}

void ATrackedTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RotateTurretToCamera(DeltaTime);
}

void ATrackedTank::RotateTurretToCamera(float DeltaTime)
{
	const FRotator CameraRotator = CameraSceneComponent->GetComponentRotation();
	const FRotator TurretRotator = TurretSceneComponent->GetComponentRotation();
	
	if (CameraRotator.Yaw != TurretRotator.Yaw)
	{
		const float DifferenceAngle = CameraRotator.Yaw - TurretRotator.Yaw;
		const float MaxRotationAngle = TurretRotationSpeed * DeltaTime;

		const float RotationAngle = FMath::Min(MaxRotationAngle, FMath::Abs(DifferenceAngle)) * FMath::Sign(DifferenceAngle);
		TurretSceneComponent->AddLocalRotation(FRotator{ 0, RotationAngle, 0 });
	}
}

void ATrackedTank::StartFire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StartFire();
	}
}

void ATrackedTank::StopFire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
	}
}
