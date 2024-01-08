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
		
	const bool bIsMoving = !SoldierCharacter->GetVelocity().IsNearlyZero();
	if (bIsMoving)
	{
		UpdateMovingAnimation();

		Montage_Stop(0, TurningSide == ECharacterTurnSide::Left ? TurnLeftMontage : TurnRightMontage);
		
		TurningSide = ECharacterTurnSide::None;
		CurrentTurnDuration = 0.f;
		CurrentTurnAngle = 0.f;
	}
	else
	{
		UpdateIdleAnimation();
		UpdateTurningAnimation(DeltaTime);
	}
}

void UST_BaseSoldierAnimInstance::UpdateMovingAnimation()
{
	const float CameraYaw = SoldierCharacter->GetCameraYawAngle();
	const float VelocityYaw = UKismetMathLibrary::MakeRotFromX(SoldierCharacter->GetVelocity()).Yaw;

	const float AbsoluteYawWalkOffset = VelocityYaw - CameraYaw;
	YawWalkOffset = FMath::Abs(AbsoluteYawWalkOffset) > 180.f ? ((360.f - FMath::Abs(AbsoluteYawWalkOffset)) * (-1 * FMath::Sign(AbsoluteYawWalkOffset))) : AbsoluteYawWalkOffset;

	YawAimOffset = 0.f;
}

void UST_BaseSoldierAnimInstance::UpdateIdleAnimation()
{
	const float CameraYaw = SoldierCharacter->GetCameraYawAngle();
	const float RootYaw = SoldierCharacter->GetRootComponent()->GetComponentRotation().Yaw;
	
	const float AbsoluteYawAimOffset = CameraYaw - RootYaw;
	YawAimOffset = FMath::Abs(AbsoluteYawAimOffset) > 180.f ? ((360.f - FMath::Abs(AbsoluteYawAimOffset)) * (-1 * FMath::Sign(AbsoluteYawAimOffset))) : AbsoluteYawAimOffset;
	YawWalkOffset = 0.f;

	// Start montage animation for turning in place in case if aim offset is larger than limit. 
	if (FMath::Abs(YawAimOffset) > MaxYawAimOffset)
	{
		const AActor* OwningActor = GetOwningActor();
		UAnimMontage* NeededAnimMontage = YawAimOffset > 0 ? TurnRightMontage : TurnLeftMontage;
		if (OwningActor && NeededAnimMontage && !Montage_IsPlaying(NeededAnimMontage))
		{
			CurrentTurnDuration = Montage_Play(NeededAnimMontage);

			TurningSide = YawAimOffset < 0 ? ECharacterTurnSide::Left : ECharacterTurnSide::Right;
			StartingTurnYawAngle = OwningActor->GetRootComponent()->GetComponentRotation().Yaw;
		}
	}
}

void UST_BaseSoldierAnimInstance::UpdateTurningAnimation(float DeltaTime)
{
	if (TurningSide == ECharacterTurnSide::None || FMath::IsNearlyZero(CurrentTurnDuration))
	{
		return;
	}
	
	UAnimMontage* NeededAnimMontage = TurningSide == ECharacterTurnSide::Left ? TurnLeftMontage : TurnRightMontage;
	if (!Montage_IsPlaying(NeededAnimMontage))
	{
		TurningSide = ECharacterTurnSide::None;
		CurrentTurnDuration = 0.f;
		CurrentTurnAngle = 0.f;

		return;
	}

	AActor* OwningActor = GetOwningActor();
	if (!OwningActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("ST_BaseSoldierAnimInstance::UpdateTurningAnimation: Failed to get owning actor."));
		return;
	}

	const float AnglePerDeltaTime = DeltaTime * MaxYawAimOffset / CurrentTurnDuration;
	
	FRotator ActorRotation = OwningActor->GetActorRotation();
	ActorRotation.Yaw += AnglePerDeltaTime * (TurningSide == ECharacterTurnSide::Left ? -1 : 1);
	CurrentTurnAngle += AnglePerDeltaTime;

	const float RemainingTurnAngle = MaxYawAimOffset - CurrentTurnAngle;
	if (FMath::Abs(YawAimOffset) > RemainingTurnAngle)
	{
		// in case if yaw offset is bigger than remaining rotation angle - rotate the actor additionaly. 
		ActorRotation.Yaw += (FMath::Abs(YawAimOffset) - RemainingTurnAngle) * (TurningSide == ECharacterTurnSide::Left ? -1 : 1);
	}

	OwningActor->SetActorRotation(ActorRotation);
}

void UST_BaseSoldierAnimInstance::OnWeaponEquipped(int32 WeaponIndex, AST_BaseWeapon* Weapon)
{
	bIsWeaponEquipped = true;
}
