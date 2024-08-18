#include "Actors/Characters/Soldiers/ST_BaseSoldierAnimInstance.h"

#include "Actors/Characters/Soldiers/ST_BaseSoldierCharacter.h"
#include "Actors/Characters/Soldiers/ST_SoldierAnimDataAsset.h"
#include "Actors/Weapons/ST_BaseWeapon.h"
#include "Components/ST_SoldierMovementComponent.h"
#include "Components/Weapons/ST_BaseWeaponsManagerComponent.h"
#include "Core/Animation/ST_AnimNotify.h"
#include "Core/ST_CoreTypes.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Subsystems/HealthSubsystem/ST_HealthComponent.h"
#include "TimerManager.h"

void UST_BaseSoldierAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	MovingType = ECharacterMovingType::Standing;
	SoldierCharacter = Cast<AST_BaseSoldierCharacter>(TryGetPawnOwner());
	if (!SoldierCharacter)
	{
		return;
	}

	RightHandSocketName = SoldierCharacter->GetRightHandSocketName();
	LeftHandSocketName = SoldierCharacter->GetLeftHandSocketName();
	SecondWeaponSocketName = SoldierCharacter->GetSecondWeaponSocketName();

	if (UST_SoldierMovementComponent* MovementComponent = Cast<UST_SoldierMovementComponent>(SoldierCharacter->GetMovementComponent()))
	{
		MovementComponent->OnMovingTypeChanged.BindUObject(this, &ThisClass::OnMovementTypeChanged);
	}

	TArray<UActorComponent*> WeaponManagerComponents;
	SoldierCharacter->GetComponents(UST_BaseWeaponsManagerComponent::StaticClass(), WeaponManagerComponents);
	if (WeaponManagerComponents.Num() > 0)
	{
		WeaponManagerComponent = Cast<UST_BaseWeaponsManagerComponent>(WeaponManagerComponents[0]);
		if (WeaponManagerComponent)
		{
			WeaponManagerComponent->OnWeaponAdded.AddUObject(this, &ThisClass::OnWeaponEquipped);
			WeaponManagerComponent->OnWeaponFiredDelegate.BindUObject(this, &ThisClass::OnWeaponFired);
			WeaponManagerComponent->OnWeaponReloadingStartedDelegate.AddUObject(this, &ThisClass::OnWeaponReloading);
			WeaponManagerComponent->OnWeaponSwitchedDelegate.AddUObject(this, &ThisClass::OnWeaponSwitched);
		}
	}

	UST_HealthComponent* HealthComponent = SoldierCharacter->GetComponentByClass<UST_HealthComponent>();
	if (HealthComponent)
	{
		HealthComponent->OnDamageDealedDelegate.AddUObject(this, &ThisClass::OnDamageDealed);
	}
}

void UST_BaseSoldierAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (!SoldierCharacter || !SoldierCharacter->GetMesh())
	{
		return;
	}
		
	if (!MontagesDataAsset)
	{
		return;
	}

	// If Montage from default slot is playing at the moment - we don't need to update anything else;
	if (GetCurrentActiveMontage() && GetCurrentActiveMontage()->SlotAnimTracks[0].SlotName == FName("DefaultSlot"))
	{
		return;
	}

	const bool bIsMoving = !SoldierCharacter->GetVelocity().IsNearlyZero();
	if (bIsMoving)
	{
		UpdateMovingAnimation();

		Montage_Stop(0, TurningSide == ECharacterTurnSide::Left ? MontagesDataAsset->TurnLeftMontage : MontagesDataAsset->TurnRightMontage);
		
		TurningSide = ECharacterTurnSide::None;
		CurrentTurnDuration = 0.f;
		CurrentTurnAngle = 0.f;
	}
	else
	{
		UpdateIdleAnimation();
		UpdateTurningAnimation(DeltaTime);
	}

	if (bIsWeaponEquipped)
	{
		UpdateLeftHandWeaponPosition();
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
		UAnimMontage* NeededAnimMontage = YawAimOffset > 0 ? MontagesDataAsset->TurnRightMontage : MontagesDataAsset->TurnLeftMontage;
		if (OwningActor && NeededAnimMontage && !Montage_IsPlaying(NeededAnimMontage))
		{
			CurrentTurnDuration = Montage_Play(NeededAnimMontage, 1.f, EMontagePlayReturnType::MontageLength, 0.0f, false);

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
	
	UAnimMontage* NeededAnimMontage = TurningSide == ECharacterTurnSide::Left ? MontagesDataAsset->TurnLeftMontage : MontagesDataAsset->TurnRightMontage;
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

void UST_BaseSoldierAnimInstance::UpdateLeftHandWeaponPosition()
{
	if (!CurrentWeapon)
	{
		return;
	}

	TArray<UActorComponent*> AttachComponents = CurrentWeapon->GetComponentsByTag(USceneComponent::StaticClass(), FName("LeftHandAttach"));
	if (AttachComponents.Num() <= 0)
	{
		return;
	}
	
	if (USceneComponent* LeftHandAttachComponent = Cast<USceneComponent>(AttachComponents[0]))
	{
		FVector OutLocation;
		FRotator OutRotation;
		SoldierCharacter->GetMesh()->TransformToBoneSpace(RightHandBoneName, LeftHandAttachComponent->GetComponentLocation(), FRotator::ZeroRotator, OutLocation, OutRotation);
	
		LeftHandTransform.SetLocation(OutLocation);
		LeftHandTransform.SetRotation(FQuat(OutRotation));
	}
}

void UST_BaseSoldierAnimInstance::SetupCurrentWeapon()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->SetWeaponEnabled(true);

		TArray<UActorComponent*> MagazineComponents = CurrentWeapon->GetComponentsByTag(UStaticMeshComponent::StaticClass(), FName("Magazine"));
		if (MagazineComponents.Num() > 0)
		{
			CurrentMagazineComponent = Cast<UStaticMeshComponent>(MagazineComponents[0]);
			CurrentMagazineTransform = CurrentMagazineComponent->GetRelativeTransform();
		}
	}
}

void UST_BaseSoldierAnimInstance::OnWeaponEquipped(int32 WeaponIndex, AST_BaseWeapon* Weapon)
{
	bIsWeaponEquipped = true;

	if (WeaponManagerComponent->GetCurrentWeaponIndex() == WeaponIndex)
	{
		CurrentWeapon = Weapon;
		SetupCurrentWeapon();
	}
	else if (WeaponManagerComponent->GetWeapons().Num() == 2)
	{
		AdditionalWeapon = Weapon;
	}
}

void UST_BaseSoldierAnimInstance::OnWeaponFired(AST_BaseWeapon* Weapon)
{
    if (!bIsReloading && MontagesDataAsset && MontagesDataAsset->TwoHandsWeaponFireMontage)
    {
        Montage_Play(MontagesDataAsset->TwoHandsWeaponFireMontage);
    }
}

void UST_BaseSoldierAnimInstance::OnWeaponReloading(AST_BaseWeapon* Weapon)
{
	if (bIsReloading || !MontagesDataAsset)
	{
		return;
	}

	const float MontageLength = Montage_Play(MontagesDataAsset->TwoHandsWeaponReloadingMontage);

	bIsReloading = true;
	auto OnReloadingMontageFinished = [this]()
	{
		bIsReloading = false;
	};

	FTimerHandle ReloadingTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(ReloadingTimerHandle, FTimerDelegate::CreateLambda(OnReloadingMontageFinished), MontageLength, false);
}

void UST_BaseSoldierAnimInstance::OnWeaponSwitched(int32 PreviousWeaponIndex, int32 NewWeaponIndex)
{
	if (!MontagesDataAsset || !WeaponManagerComponent)
	{
		return;
	}

	SwitchingWeapon = WeaponManagerComponent->GetWeapon(NewWeaponIndex);
	if (SwitchingWeapon)
	{
		SwitchingWeapon->SetWeaponEnabled(false);

		if (bIsReloading)
		{
			Montage_Stop(0.f, MontagesDataAsset->TwoHandsWeaponReloadingMontage);
			InternalAnimNotify_OnMagazineInserted();

			CurrentWeapon->InterruptReloading();
		}

		const float MontageLength = Montage_Play(MontagesDataAsset->SwitchWeaponMontage);
		
		bIsWeaponSwitching = true;
		FTimerHandle SwitchingTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(SwitchingTimerHandle, this, &ThisClass::OnWeaponEquippingAnimationFinished, MontageLength - 0.8f, false);
	}
}

void UST_BaseSoldierAnimInstance::OnDamageDealed(float CurrentHealthValue)
{
	if (bIsReloading || !MontagesDataAsset)
	{
		return;
	}

	if (FMath::IsNearlyZero(CurrentHealthValue))
	{
		bIsDead = true;
	}
	else if (!Montage_IsPlaying(MontagesDataAsset->HitReactionMontage) && CurrentHealthValue > 0)
	{
		Montage_Play(MontagesDataAsset->HitReactionMontage);
	}
}

void UST_BaseSoldierAnimInstance::InternalAnimNotify_OnMagazineGrabbed()
{
	if (!SoldierCharacter || !SoldierCharacter->GetMesh())
	{
		return;
	}

	if (!CurrentMagazineComponent)
	{
		return;
	}
	
	CurrentMagazineComponent->DetachFromParent();
	CurrentMagazineComponent->AttachToComponent(SoldierCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, LeftHandSocketName);
}

void UST_BaseSoldierAnimInstance::InternalAnimNotify_OnMagazineInserted()
{
	if (!CurrentWeapon)
	{
		return;
	}

	if (!CurrentMagazineComponent)
	{
		return;
	}

	CurrentMagazineComponent->DetachFromParent();
	CurrentMagazineComponent->AttachToComponent(CurrentWeapon->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	CurrentMagazineComponent->SetRelativeTransform(CurrentMagazineTransform);
}

void UST_BaseSoldierAnimInstance::InternalAnimNotify_OnWeaponSwitched()
{
	if (!CurrentWeapon || !WeaponManagerComponent)
	{
		return;
	}

	CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);

	if (AdditionalWeapon && SwitchingWeapon == AdditionalWeapon)
	{	
		AdditionalWeapon->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
		CurrentWeapon->AttachToComponent(SoldierCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, SecondWeaponSocketName);
		AdditionalWeapon->AttachToComponent(SoldierCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, RightHandSocketName);
	
		Swap(CurrentWeapon, AdditionalWeapon);
	}
	else
	{
		CurrentWeapon->SetHidden(true);
		SwitchingWeapon->AttachToComponent(SoldierCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, RightHandSocketName);
		SwitchingWeapon->SetHidden(false);

		CurrentWeapon = SwitchingWeapon;
	}

	SetupCurrentWeapon();
}

void UST_BaseSoldierAnimInstance::OnWeaponEquippingAnimationFinished()
{
	bIsWeaponSwitching = false;

	OnEqiupWeaponAnimationFinishedDelegate.ExecuteIfBound();
}
