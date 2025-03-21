#include "Actions/PossessVehicleAction.h"

#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "InteractingComponent.h"
#include "InteractionComponent.h"
#include "InteractionPointComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Templates/Function.h"

UPossessVehicleAction::UPossessVehicleAction()
{
	bShouldBeDeactivated = true;
}

bool UPossessVehicleAction::Activate(UInteractingComponent* InteractingComponent, UInteractionComponent* InteractionComponent)
{
	if (!CanBeActivated(InteractingComponent, InteractionComponent))
	{
		return false;
	}

	APawn* CharacterPawn = Cast<APawn>(InteractingComponent->GetOwner());
	APawn* VehiclePawn = Cast<APawn>(InteractionComponent->GetOwner());

	InteractionComponent->SetIsComponentActive(false);
	const UInteractionPointComponent* InteractionPoint = InteractionComponent->GetClosestInteractionPoint(CharacterPawn->GetActorLocation());
	if (InteractionPoint)
	{
		FVector InteractionLocation = InteractionPoint->GetComponentLocation();
		InteractionLocation.Z = CharacterPawn->GetActorLocation().Z;

		CharacterPawn->SetActorLocation(InteractionLocation);
		CharacterPawn->SetActorRotation(InteractionPoint->GetComponentRotation());
	}

	CharacterPawn->DisableInput(nullptr);

	PlayActionMontage(CharacterPawn, VehiclePawn);

	return true;
}

bool UPossessVehicleAction::Deactivate(UInteractingComponent* InteractingComponent, UInteractionComponent* InteractionComponent)
{
	APawn* VehiclePawn = Cast<APawn>(InteractionComponent->GetOwner());
	APawn* SoldierPawn = Cast<APawn>(InteractingComponent->GetOwner());
	if (!SoldierPawn || !VehiclePawn)
	{
		return false;
	}

	AController* Controller = VehiclePawn->GetController();
	if (!Controller)
	{
		return false;
	}

	auto IsLocationFree = [this, VehiclePawn](AActor* Actor, const float ActorCollisionRadius, const float ActorCollisionHelfheight, const UInteractionPointComponent* InteractionPoint) -> bool
	{
		if (!Actor)
		{
			return false;
		}

		TArray<AActor*> OutActors;
		return !UKismetSystemLibrary::CapsuleOverlapActors(this, InteractionPoint->GetComponentLocation(), ActorCollisionRadius, ActorCollisionHelfheight, {}, nullptr, { VehiclePawn }, OutActors);
	};

	float SoldierCollisionRadius;
	float SoldierCollisionHalfLength;
	UCapsuleComponent* CapsuleComponent = Cast<UCapsuleComponent>(SoldierPawn->GetRootComponent());
	if (!CapsuleComponent)
	{
		return false;
	}

	CapsuleComponent->GetScaledCapsuleSize(SoldierCollisionRadius, SoldierCollisionHalfLength);

	const UInteractionPointComponent* UsingInteractionPoint = nullptr;
	for (const UInteractionPointComponent* InteractionPoint : InteractionComponent->GetInteractionPoints())
	{
		if (IsLocationFree(SoldierPawn, SoldierCollisionRadius, SoldierCollisionHalfLength, InteractionPoint))
		{
			UsingInteractionPoint = InteractionPoint;
			break;
		}
	}

	if (!IsValid(UsingInteractionPoint))
	{
		return false;
	}

	FVector InteractionLocation = UsingInteractionPoint->GetComponentLocation() + UsingInteractionPoint->GetForwardVector() * UsingInteractionPoint->GetInteractionDistance();
	InteractionLocation.Z = SoldierPawn->GetActorLocation().Z;

	SoldierPawn->AttachToActor(VehiclePawn, FAttachmentTransformRules{EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, false});
	SoldierPawn->SetActorLocation(InteractionLocation, true);
	SoldierPawn->SetActorRotation(UsingInteractionPoint->GetComponentRotation() * (-1));
	
	SoldierPawn->GetRootComponent()->SetVisibility(true, true);

	VehiclePawn->DisableInput(Cast<APlayerController>(Controller));

	PlayDeactivationMontage(SoldierPawn, VehiclePawn, InteractionComponent, UsingInteractionPoint);
	return true;
}

bool UPossessVehicleAction::CanBeActivated(UInteractingComponent* InteractingComponent, UInteractionComponent* InteractionComponent) const
{
	if (!InteractingComponent || !InteractionComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("UPossessVehicleAction::Activate: Failed to get InteractingComponent or InteractionComponent"));
		return false;
	}

	APawn* CharacterPawn = Cast<APawn>(InteractingComponent->GetOwner());
	APawn* VehiclePawn = Cast<APawn>(InteractionComponent->GetOwner());
	if (!CharacterPawn || !VehiclePawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("UPossessVehicleAction::Activate: Owners of components are not pawns."));
		return false;
	}

	AController* CharacterController = CharacterPawn->GetController();
	if (CharacterController == nullptr || VehiclePawn->GetController() != nullptr)
	{
		return false;
	}
	
	return true;
}

FText UPossessVehicleAction::GetActionDescription() const
{
	return FText::FromString(TEXT("Possess vehicle"));
}

void UPossessVehicleAction::PlayActionMontage(APawn* CharacterPawn, APawn* VehiclePawn) const
{
	auto OnActionMontagePlayed = [CharacterPawn, VehiclePawn]()
	{
		CharacterPawn->GetRootComponent()->SetVisibility(false, true);
		CharacterPawn->SetActorEnableCollision(false);
		CharacterPawn->EnableInput(nullptr);

		AController* CharacterController = CharacterPawn->GetController();

		// TODO: add SetViewTargetWithBlend for new camera
		CharacterController->Possess(VehiclePawn);
	};

	PlayMontage_Internal(CharacterPawn, VehiclePawn, ActionMontage, (-0.25f), OnActionMontagePlayed);
}

void UPossessVehicleAction::PlayDeactivationMontage(APawn* CharacterPawn, APawn* VehiclePawn, UInteractionComponent* InteractionComponent, const UInteractionPointComponent* InteractionPoint) const
{
	auto OnActionMontagePlayed = [CharacterPawn, VehiclePawn, InteractionComponent, InteractionPoint]()
	{
		if (AController* Controller = VehiclePawn->GetController())
		{
			InteractionComponent->SetIsComponentActive(true);
			CharacterPawn->SetActorEnableCollision(true);
			CharacterPawn->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			CharacterPawn->SetActorLocation(InteractionPoint->GetComponentLocation());

			VehiclePawn->EnableInput(nullptr);

			Controller->Possess(CharacterPawn);
		}
	};

	PlayMontage_Internal(CharacterPawn, VehiclePawn, DeactivationMontage, 0.f, OnActionMontagePlayed);
}

void UPossessVehicleAction::PlayMontage_Internal(APawn* CharacterPawn, APawn* VehiclePawn, UAnimMontage* AnimMontage, float Delay, TFunction<void()> OnMontageCompleted) const
{
	if (!AnimMontage)
	{
		return;
	}

	USkeletalMeshComponent* SkeletalMesh = CharacterPawn->GetComponentByClass<USkeletalMeshComponent>();
	if (!SkeletalMesh)
	{
		return;
	}

	UAnimInstance* AnimInstance = SkeletalMesh->GetAnimInstance();
	if (!AnimInstance)
	{
		return;
	}

	const float MontageLength = AnimInstance->Montage_Play(AnimMontage, 1.f, EMontagePlayReturnType::MontageLength, 0.f, true);

	FTimerHandle ActionTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(ActionTimerHandle, FTimerDelegate::CreateLambda(OnMontageCompleted), MontageLength + Delay, false);
}
