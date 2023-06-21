#include "Actors/Pawns/ST_BaseVehicle.h"

#include "Abilities/GameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Gameplay/ST_GameplayGameState.h"
#include "Engine/World.h"
#include "Systems/GameplayAbilitySystem/ST_VehicleAbilitySystemComponent.h"


AST_BaseVehicle::AST_BaseVehicle()
{
	AbilitySystemComponent = CreateDefaultSubobject<UST_VehicleAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
}

void AST_BaseVehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
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

void AST_BaseVehicle::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	SetOwner(NewController);
}

void AST_BaseVehicle::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}