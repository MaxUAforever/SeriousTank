#include "Actors/Characters/Soldiers/ST_BaseSoldierAnimInstance.h"

#include "Actors/Characters/Soldiers/ST_BaseSoldierCharacter.h"
#include "Components/ST_SoldierMovementComponent.h"
#include "Components/Weapons/ST_BaseWeaponsManagerComponent.h"
#include "Core/ST_CoreTypes.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"

void UST_BaseSoldierAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	MovingType = ECharacterMovingType::Standing;
	SoldierCharacter = Cast<AST_BaseSoldierCharacter>(TryGetPawnOwner());
	if (!SoldierCharacter)
	{
		return;
	}

	if (UST_SoldierMovementComponent* MovementComponent = Cast<UST_SoldierMovementComponent>(SoldierCharacter->GetMovementComponent()))
	{
		MovementComponent->OnMovingTypeChanged.BindUObject(this, &ThisClass::OnMovementTypeChanged);
	}
	
	TArray<UActorComponent*> WeaponManagerComponents;
	SoldierCharacter->GetComponents(UST_BaseWeaponsManagerComponent::StaticClass(), WeaponManagerComponents);
	if (WeaponManagerComponents.Num() > 0)
	{
		if (UST_BaseWeaponsManagerComponent* WeaponManagerComponent = Cast<UST_BaseWeaponsManagerComponent>(WeaponManagerComponents[0]))
		{
			WeaponManagerComponent->OnWeaponAdded.AddUObject(this, &ThisClass::OnWeaponEquipped);
		}
	}
}

void UST_BaseSoldierAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (!SoldierCharacter || !SoldierCharacter->GetMesh())
	{
		return;
	}
		
	const float CameraYaw = SoldierCharacter->GetCameraYawAngle();
		
	const bool bIsMoving = !SoldierCharacter->GetVelocity().IsNearlyZero();
	if (bIsMoving)
	{
		const float VelocityYaw = UKismetMathLibrary::MakeRotFromX(SoldierCharacter->GetVelocity()).Yaw;
		YawWalkOffset = VelocityYaw - CameraYaw;
		YawAimOffset = 0.f;
	}
	else
	{
		const float RootYaw = SoldierCharacter->GetRootComponent()->GetComponentRotation().Yaw;
		YawAimOffset = CameraYaw - RootYaw;
		YawWalkOffset = 0.f;

		if (FMath::Abs(YawAimOffset) > MaxYawAimOffset)
		{
			OnMaxAimAngleReached.ExecuteIfBound(YawAimOffset);
		}
	}
}

void UST_BaseSoldierAnimInstance::OnWeaponEquipped(int32 WeaponIndex, AST_BaseWeapon* Weapon)
{
	bIsWeaponEquipped = true;
}
