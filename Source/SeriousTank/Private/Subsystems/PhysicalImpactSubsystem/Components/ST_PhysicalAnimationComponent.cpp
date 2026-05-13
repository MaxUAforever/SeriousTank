#include "Subsystems/PhysicalImpactSubsystem/Components/ST_PhysicalAnimationComponent.h"

// Game includes
#include "Subsystems/PhysicalImpactSubsystem/ST_PhysicalImpactTypes.h"

// Engine includes
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"

UST_PhysicalAnimationComponent::UST_PhysicalAnimationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UST_PhysicalAnimationComponent::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* CharacterOwner = Cast<ACharacter>(GetOwner());
	if (!IsValid(CharacterOwner))
	{
		UE_LOG(LogPhysicalImpact, Warning, TEXT("%s: UST_PhysicalAnimationComponent is only compatible with ACharacter actors. Component will not function properly."), ANSI_TO_TCHAR(__FUNCTION__));
		return;
	}

	SetComponentTickEnabled(false);
	SetSkeletalMeshComponent(CharacterOwner->GetMesh());

	UpdateAnimationProperties({ DefaultProfileName, DefaultImpactBoneName, bDefaultIncludeBone });
}

void UST_PhysicalAnimationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	USkeletalMeshComponent* SkeletalMesh = GetSkeletalMesh();
	if (!IsValid(SkeletalMesh))
	{
		UE_LOG(LogPhysicalImpact, Warning, TEXT("%s: SkeletalMeshComponent is not valid!"), ANSI_TO_TCHAR(__FUNCTION__));
		SetComponentTickEnabled(false);
		return;
	}

	RemainingImpactReactionTime -= DeltaTime;
	if (RemainingImpactReactionTime <= 0.f)
	{
		SkeletalMesh->SetAllBodiesBelowSimulatePhysics(CurrentAnimationProperties.BoneName, false, CurrentAnimationProperties.bIncludeBone);
		SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		SetComponentTickEnabled(false);
		return;
	}

	SkeletalMesh->SetAllBodiesBelowPhysicsBlendWeight(CurrentAnimationProperties.BoneName, RemainingImpactReactionTime / DefaultImpactReactionTime, CurrentAnimationProperties.bIncludeBone);
}

void UST_PhysicalAnimationComponent::SetEnabled(bool bInIsEnabled)
{
	bIsEnabled = bInIsEnabled;
	RemainingImpactReactionTime = 0.f;
}

void UST_PhysicalAnimationComponent::UpdateAnimationProperties(const FST_PhysicalAnimationProperties& AnimationProperties)
{
	CurrentAnimationProperties = AnimationProperties;
	ApplyPhysicalAnimationProfileBelow(CurrentAnimationProperties.BoneName, CurrentAnimationProperties.ProfileName, CurrentAnimationProperties.bIncludeBone);
}

void UST_PhysicalAnimationComponent::ApplyPhysicalImpact(const FST_PhysicalImpactParameters& ImpactParameters, float ImpactForce)
{
	if (!bIsEnabled)
	{
		return;
	}

	USkeletalMeshComponent* SkeletalMesh = GetSkeletalMesh();
	if (!IsValid(SkeletalMesh))
	{
		UE_LOG(LogPhysicalImpact, Warning, TEXT("%s: SkeletalMeshComponent is not valid!"), ANSI_TO_TCHAR(__FUNCTION__));
		return;
	}

	if (ImpactForce <= 0.f)
	{
		UE_LOG(LogPhysicalImpact, Warning, TEXT("%s: ImpactForce must be greater than 0!"), ANSI_TO_TCHAR(__FUNCTION__));
		return;
	}

	SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SkeletalMesh->SetAllBodiesBelowSimulatePhysics(CurrentAnimationProperties.BoneName, true, CurrentAnimationProperties.bIncludeBone);
	SkeletalMesh->AddImpulseAtLocation(ImpactParameters.ImpactForceDirection * ImpactForce, ImpactParameters.ImpactLocation, CurrentAnimationProperties.BoneName);

	RemainingImpactReactionTime = DefaultImpactReactionTime;
	SetComponentTickEnabled(true);
}
