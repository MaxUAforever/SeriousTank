#include "Actions/BaseAnimatedAction.h"

#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Data/Actions/BaseAnimatedActionDataAsset.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Components/InteractingComponent.h"
#include "Components/InteractionComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Templates/Function.h"

void UBaseAnimatedAction::BeginDestroy()
{
	Super::BeginDestroy();

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(ActionTimerHandle);
	}
}

void UBaseAnimatedAction::Initialize(const UBaseInteractionActionDataAsset* InteractionActionDataAsset)
{
	Super::Initialize(InteractionActionDataAsset);

	const UBaseAnimatedActionDataAsset* AnimatedActionDataAsset = Cast<UBaseAnimatedActionDataAsset>(InteractionActionDataAsset);
	if (!IsValid(AnimatedActionDataAsset))
	{
		return;
	}

	ActivationMontageCompleteDelay = AnimatedActionDataAsset->ActivationMontageCompleteDelay;
	DeactivationMontageCompleteDelay = AnimatedActionDataAsset->DeactivationMontageCompleteDelay;
}

bool UBaseAnimatedAction::Activate(UInteractingComponent* InteractingComponent, UInteractionComponent* InteractionComponent)
{
	if (!Super::Activate(InteractingComponent, InteractionComponent))
	{
		return false;
	}

	UAnimInstance* AnimInstance = GetOwnerAnimInstance();
	if (!IsValid(AnimInstance) || AnimInstance->Montage_IsPlaying(GetActivationMontage()) || AnimInstance->Montage_IsPlaying(GetDeactivationMontage()))
	{
		return false;
	}

	OnActivationStarted();
	PlayActivationMontage();

	return true;
}

bool UBaseAnimatedAction::Deactivate(UInteractingComponent* InteractingComponent, UInteractionComponent* InteractionComponent)
{
	if (!Super::Deactivate(InteractingComponent, InteractionComponent))
	{
		return false;
	}

	UAnimInstance* AnimInstance = GetOwnerAnimInstance();
	if (!IsValid(AnimInstance) || AnimInstance->Montage_IsPlaying(GetActivationMontage()) || AnimInstance->Montage_IsPlaying(GetDeactivationMontage()))
	{
		return false;
	}

	OnDeactivationStarted();
	PlayDeactivationMontage();

	return true;
}

void UBaseAnimatedAction::PlayActivationMontage()
{
	auto OnActionMontagePlayed = [this]()
	{
		OnActivationMontagePlayed();
	};

	PlayMontage_Internal(GetActivationMontage(), ActivationMontageCompleteDelay, OnActionMontagePlayed);
}

void UBaseAnimatedAction::PlayDeactivationMontage()
{
	auto OnActionMontagePlayed = [this]()
	{
		OnDeactivationMontagePlayed();
	};

	PlayMontage_Internal(GetDeactivationMontage(), DeactivationMontageCompleteDelay, OnActionMontagePlayed);
}

void UBaseAnimatedAction::PlayMontage_Internal(UAnimMontage* AnimMontage, float Delay, TFunction<void()> OnMontageCompleted)
{
	UWorld* World = GetWorld();
	UAnimInstance* AnimInstance = GetOwnerAnimInstance();
	if (!IsValid(AnimInstance) || !IsValid(World))
	{
		return;
	}
	
	const float MontageLength = AnimInstance->Montage_Play(AnimMontage, 1.f, EMontagePlayReturnType::MontageLength, 0.f, true);

	World->GetTimerManager().SetTimer(ActionTimerHandle, FTimerDelegate::CreateLambda(OnMontageCompleted), MontageLength + Delay, false);
}

UAnimInstance* UBaseAnimatedAction::GetOwnerAnimInstance()
{
	const UInteractingComponent* InteractingComponent = GetInteractingComponent();
	if (!IsValid(InteractingComponent))
	{
		return nullptr;
	}

	AActor* InteractingActor = Cast<AActor>(InteractingComponent->GetOwner());
	if (!IsValid(InteractingActor))
	{
		return nullptr;
	}

	USkeletalMeshComponent* SkeletalMesh = InteractingActor->GetComponentByClass<USkeletalMeshComponent>();
	if (!IsValid(SkeletalMesh))
	{
		return nullptr;
	}

	return SkeletalMesh->GetAnimInstance();
}
