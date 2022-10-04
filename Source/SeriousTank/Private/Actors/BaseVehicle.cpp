#include "Actors/BaseVehicle.h"

#include "Components/InputComponent.h"

void ABaseVehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ThisClass::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ThisClass::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("RotateCamera"), this, &ThisClass::RotateCamera);

	PlayerInputComponent->BindAction("StartFire", EInputEvent::IE_Pressed, this, &ThisClass::StartFire);
	PlayerInputComponent->BindAction("StopFire", EInputEvent::IE_Released, this, &ThisClass::StopFire);
	
	PlayerInputComponent->BindAction("FirstWeapon", EInputEvent::IE_Pressed, this, &ThisClass::SwitchToFirstWeapon);
	PlayerInputComponent->BindAction("SecondWeapon", EInputEvent::IE_Pressed, this, &ThisClass::SwitchToSecondWeapon);
	PlayerInputComponent->BindAction("ThirdWeapon", EInputEvent::IE_Pressed, this, &ThisClass::SwitchToThirdWeapon);
}

