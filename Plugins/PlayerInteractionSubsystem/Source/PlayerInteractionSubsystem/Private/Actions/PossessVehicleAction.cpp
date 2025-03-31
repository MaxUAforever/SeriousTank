#include "Actions/PossessVehicleAction.h"

#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Components/InteractingComponent.h"
#include "Components/InteractionComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Templates/Function.h"

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

void UPossessVehicleAction::OnActivationStarted()
{
	UInteractingComponent* InteractingComponent = GetInteractingComponent();
	UInteractionComponent* InteractionComponent = GetInteractionComponent();
	if (!IsValid(InteractingComponent) || !IsValid(InteractionComponent))
	{
		return;
	}

	CachedCharacterPawn = Cast<APawn>(InteractingComponent->GetOwner());
	CachedVehiclePawn = Cast<APawn>(InteractionComponent->GetOwner());

	InteractionComponent->SetIsInteractionComponentActive(false);
	const TOptional<FTransform> InteractionPoint = InteractionComponent->GetClosestInteractionPoint(CachedCharacterPawn->GetActorLocation());
	if (InteractionPoint.IsSet())
	{
		FVector InteractionLocation = InteractionPoint.GetValue().GetLocation();
		InteractionLocation.Z = CachedCharacterPawn->GetActorLocation().Z;

		CachedCharacterPawn->SetActorLocation(InteractionLocation);
		CachedCharacterPawn->SetActorRotation(InteractionPoint.GetValue().GetRotation());
	}

	CachedCharacterPawn->DisableInput(nullptr);
}

void UPossessVehicleAction::OnDeactivationStarted()
{
	UInteractingComponent* InteractingComponent = GetInteractingComponent();
	UInteractionComponent* InteractionComponent = GetInteractionComponent();
	if (!IsValid(InteractingComponent) || !IsValid(InteractionComponent))
	{
		return;
	}

	CachedCharacterPawn = Cast<APawn>(InteractingComponent->GetOwner());
	CachedVehiclePawn = Cast<APawn>(InteractionComponent->GetOwner());
	if (!IsValid(CachedCharacterPawn) || !IsValid(CachedVehiclePawn))
	{
		return;
	}

	AController* Controller = CachedVehiclePawn->GetController();
	if (!Controller)
	{
		return;
	}

	auto IsLocationFree = [this](AActor* Actor, const float ActorCollisionRadius, const float ActorCollisionHelfheight, const FVector& InteractionPoint) -> bool
	{
		if (!Actor)
		{
			return false;
		}

		TArray<AActor*> OutActors;
		return !UKismetSystemLibrary::CapsuleOverlapActors(this, InteractionPoint, ActorCollisionRadius, ActorCollisionHelfheight, {}, nullptr, { CachedVehiclePawn }, OutActors);
	};

	float SoldierCollisionRadius;
	float SoldierCollisionHalfLength;
	UCapsuleComponent* CapsuleComponent = Cast<UCapsuleComponent>(CachedCharacterPawn->GetRootComponent());
	if (!CapsuleComponent)
	{
		return;
	}

	CapsuleComponent->GetScaledCapsuleSize(SoldierCollisionRadius, SoldierCollisionHalfLength);

	TOptional<FTransform> UsingInteractionPoint;
	for (const FTransform& InteractionPoint : InteractionComponent->GetWorldInteractionPoints())
	{
		if (IsLocationFree(CachedCharacterPawn, SoldierCollisionRadius, SoldierCollisionHalfLength, InteractionPoint.GetLocation()))
		{
			UsingInteractionPoint = InteractionPoint;
			break;
		}
	}

	if (!UsingInteractionPoint.IsSet())
	{
		return;
	}

	FVector InteractionLocation = UsingInteractionPoint.GetValue().GetLocation() + UsingInteractionPoint.GetValue().GetUnitAxis(EAxis::X) /** UsingInteractionPoint->GetInteractionDistance()*/;
	InteractionLocation.Z = CachedCharacterPawn->GetActorLocation().Z;

	CachedCharacterPawn->AttachToActor(CachedVehiclePawn, FAttachmentTransformRules{ EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, false });
	CachedCharacterPawn->SetActorLocation(InteractionLocation, true);
	CachedCharacterPawn->SetActorRotation(UsingInteractionPoint.GetValue().GetRotation() * (-1));

	CachedCharacterPawn->GetRootComponent()->SetVisibility(true, true);

	CachedVehiclePawn->DisableInput(Cast<APlayerController>(Controller));
}

void UPossessVehicleAction::OnActivationMontagePlayed()
{
	if (!IsValid(CachedCharacterPawn) || !IsValid(CachedVehiclePawn))
	{
		return;
	}

	CachedCharacterPawn->GetRootComponent()->SetVisibility(false, true);
	CachedCharacterPawn->SetActorEnableCollision(false);
	CachedCharacterPawn->EnableInput(nullptr);

	AController* CharacterController = CachedCharacterPawn->GetController();

	// TODO: add SetViewTargetWithBlend for new camera
	CharacterController->Possess(CachedVehiclePawn);
}

void UPossessVehicleAction::OnDeactivationMontagePlayed()
{
	UInteractionComponent* InteractionComponent = GetInteractionComponent();
	if (!IsValid(InteractionComponent) || !IsValid(CachedCharacterPawn) || !IsValid(CachedVehiclePawn))
	{
		return;
	}

	if (AController* Controller = CachedVehiclePawn->GetController())
	{
		InteractionComponent->SetIsInteractionComponentActive(true);
		CachedCharacterPawn->SetActorEnableCollision(true);
		CachedCharacterPawn->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		
		if (CachedInteractionPoint.IsSet())
		{
			CachedCharacterPawn->SetActorLocation(CachedInteractionPoint.GetValue().GetLocation());
		}

		CachedVehiclePawn->EnableInput(nullptr);

		Controller->Possess(CachedCharacterPawn);
	}
}
