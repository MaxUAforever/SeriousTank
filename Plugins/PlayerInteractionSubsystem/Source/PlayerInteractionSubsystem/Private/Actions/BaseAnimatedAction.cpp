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
	const UInteractingComponent* InteractingComponent = GetInteractingComponent();
	if (!IsValid(AnimMontage) || !IsValid(InteractingComponent))
	{
		return;
	}

	AActor* InteractingActor = Cast<AActor>(InteractingComponent->GetOwner());
	if (!IsValid(InteractingActor))
	{
		return;
	}

	USkeletalMeshComponent* SkeletalMesh = InteractingActor->GetComponentByClass<USkeletalMeshComponent>();
	if (!SkeletalMesh)
	{
		return;
	}

	UAnimInstance* AnimInstance = SkeletalMesh->GetAnimInstance();
	if (!AnimInstance)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	const float MontageLength = AnimInstance->Montage_Play(AnimMontage, 1.f, EMontagePlayReturnType::MontageLength, 0.f, true);

	World->GetTimerManager().SetTimer(ActionTimerHandle, FTimerDelegate::CreateLambda(OnMontageCompleted), MontageLength + Delay, false);
}
