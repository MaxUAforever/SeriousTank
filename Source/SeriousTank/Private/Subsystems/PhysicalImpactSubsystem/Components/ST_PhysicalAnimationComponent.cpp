#include "Subsystems/PhysicalImpactSubsystem/Components/ST_PhysicalAnimationComponent.h"

// Game includes
#include "Subsystems/PhysicalImpactSubsystem/ST_PhysicalImpactTypes.h"

// Engine includes
#include "Components/SkeletalMeshComponent.h"
#include "Curves/CurveFloat.h"
#include "GameFramework/Character.h"

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

	InitializePhysicalReactionPropertiesMap();
	SetReactionType(EST_PhysicalReactionType::None);

	bIsInitialized = true;
}

void UST_PhysicalAnimationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const FST_PhysicalAnimationProperties* CurrentAnimationProperties = PhysicalReactionPropertiesMap.Find(CurrentReactionType);
	if (CurrentAnimationProperties == nullptr)
	{
		UE_LOG(LogPhysicalImpact, Warning, TEXT("%s: No animation properties found for ReactionType %d!"), ANSI_TO_TCHAR(__FUNCTION__), static_cast<int32>(CurrentReactionType));
		StopCurrentPhysicalReaction();
		return;
	}

	if (!CurrentAnimationProperties->bIsTimedReaction)
	{
		return;
	}

	USkeletalMeshComponent* SkeletalMesh = GetSkeletalMesh();
	if (!IsValid(SkeletalMesh))
	{
		UE_LOG(LogPhysicalImpact, Warning, TEXT("%s: SkeletalMeshComponent is not valid!"), ANSI_TO_TCHAR(__FUNCTION__));
		StopCurrentPhysicalReaction();
		return;
	}

	CurrentImpactCurveTime += DeltaTime;
	if (CurrentImpactCurveTime >= ImpactCurveMaxTime)
	{
		StopCurrentPhysicalReaction();
		return;
	}

	const float ImpactForceValue = CurrentAnimationProperties->ReactionForceCurve->GetFloatValue(CurrentImpactCurveTime);
	SkeletalMesh->SetAllBodiesBelowPhysicsBlendWeight(CurrentAnimationProperties->BoneName, ImpactForceValue, false, CurrentAnimationProperties->bIncludeBone);
}

void UST_PhysicalAnimationComponent::SetReactionType(EST_PhysicalReactionType ReactionType)
{
	if (!bIsInitialized || ReactionType == CurrentReactionType)
	{
		return;
	}

	if (ReactionType == EST_PhysicalReactionType::None && !PhysicalReactionPropertiesMap.Contains(ReactionType))
	{
		StopCurrentPhysicalReaction();
		return;
	}

	if (!PhysicalReactionPropertiesMap.Contains(ReactionType))
	{
		UE_CLOG(ReactionType != EST_PhysicalReactionType::None, LogPhysicalImpact, Warning, TEXT("%s: ReactionType %d is not valid for this component!"), ANSI_TO_TCHAR(__FUNCTION__), static_cast<int32>(ReactionType));
		
		StopCurrentPhysicalReaction();
		return;
	}

	CurrentReactionType = ReactionType;

	SetStrengthMultiplyer(1.f);
	InitializeCurrentImpactProperties();

	const FST_PhysicalAnimationProperties& CurrentAnimationProperties = PhysicalReactionPropertiesMap[CurrentReactionType];
	ApplyPhysicalAnimationProfileBelow(CurrentAnimationProperties.BoneName, CurrentAnimationProperties.ProfileName, CurrentAnimationProperties.bIncludeBone);
}

void UST_PhysicalAnimationComponent::ApplyPhysicalImpact(const FST_PhysicalImpactParameters& ImpactParameters)
{
	if (!bIsInitialized)
	{
		return;
	}

	if (CurrentReactionType != EST_PhysicalReactionType::None && CurrentReactionType != ImpactParameters.ExpectedReactionType)
	{
		return;
	}

	USkeletalMeshComponent* SkeletalMesh = GetSkeletalMesh();
	if (!IsValid(SkeletalMesh))
	{
		UE_LOG(LogPhysicalImpact, Warning, TEXT("%s: SkeletalMeshComponent is not valid!"), ANSI_TO_TCHAR(__FUNCTION__));
		return;
	}

	if (ImpactParameters.ImpactForceStrength <= 0.f)
	{
		UE_LOG(LogPhysicalImpact, Warning, TEXT("%s: ImpactForce must be greater than 0!"), ANSI_TO_TCHAR(__FUNCTION__));
		return;
	}

	SetReactionType(ImpactParameters.ExpectedReactionType);

	const FST_PhysicalAnimationProperties* CurrentAnimationProperties = PhysicalReactionPropertiesMap.Find(CurrentReactionType);
	if (CurrentAnimationProperties == nullptr)
	{
		UE_LOG(LogPhysicalImpact, Warning, TEXT("%s: No animation properties found for ReactionType %d!"), ANSI_TO_TCHAR(__FUNCTION__), static_cast<int32>(CurrentReactionType));
		return;
	}

	CurrentImpactCurveTime = ImpactCurveMinTime;
	SetComponentTickEnabled(true);

	SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SkeletalMesh->SetAllBodiesBelowSimulatePhysics(CurrentAnimationProperties->BoneName, true, CurrentAnimationProperties->bIncludeBone);
	SkeletalMesh->AddImpulseAtLocation(ImpactParameters.ImpactForceDirection * ImpactParameters.ImpactForceStrength, ImpactParameters.ImpactLocation, CurrentAnimationProperties->BoneName);
}

void UST_PhysicalAnimationComponent::InitializePhysicalReactionPropertiesMap()
{
	PhysicalReactionPropertiesMap.Empty(PhysicalReactionSettingsArray.Num());
	for (const FST_PhysicalReactionSettings& ReactionSettings : PhysicalReactionSettingsArray)
	{
		PhysicalReactionPropertiesMap.Add(ReactionSettings.ReactionType, ReactionSettings.AnimationProperties);
	}
}

void UST_PhysicalAnimationComponent::InitializeCurrentImpactProperties()
{
	ImpactCurveMinTime = 0.f;
	ImpactCurveMaxTime = 0.f;
	ImpactCurveDuration = 0.f;

	const FST_PhysicalAnimationProperties* CurrentAnimationProperties = PhysicalReactionPropertiesMap.Find(CurrentReactionType);
	if (CurrentAnimationProperties == nullptr)
	{
		UE_LOG(LogPhysicalImpact, Warning, TEXT("%s: No animation properties found for ReactionType %d!"), ANSI_TO_TCHAR(__FUNCTION__), static_cast<int32>(CurrentReactionType));
		return;
	}

	if (CurrentAnimationProperties->bIsTimedReaction && CurrentAnimationProperties->ReactionForceCurve != nullptr)
	{
		CurrentAnimationProperties->ReactionForceCurve->GetTimeRange(ImpactCurveMinTime, ImpactCurveMaxTime);
		ImpactCurveDuration = FMath::Max(ImpactCurveMaxTime - ImpactCurveMinTime, 0.f);
		
		if (FMath::IsNearlyZero(ImpactCurveDuration))
		{
			UE_LOG(LogPhysicalImpact, Warning, TEXT("%s: Impact reaction duraton should be greater than 0!"), ANSI_TO_TCHAR(__FUNCTION__));
			return;
		}
	}
}

void UST_PhysicalAnimationComponent::StopCurrentPhysicalReaction()
{
	CurrentReactionType = EST_PhysicalReactionType::None;
	CurrentImpactCurveTime = ImpactCurveMaxTime;

	SetComponentTickEnabled(false);
	SetStrengthMultiplyer(0.f);

	const FST_PhysicalAnimationProperties* CurrentAnimationProperties = PhysicalReactionPropertiesMap.Find(CurrentReactionType);
	if (CurrentAnimationProperties == nullptr)
	{
		UE_LOG(LogPhysicalImpact, Warning, TEXT("%s: No animation properties found for ReactionType %d!"), ANSI_TO_TCHAR(__FUNCTION__), static_cast<int32>(CurrentReactionType));
		return;
	}

	USkeletalMeshComponent* SkeletalMesh = GetSkeletalMesh();
	if (!IsValid(SkeletalMesh))
	{
		UE_LOG(LogPhysicalImpact, Warning, TEXT("%s: SkeletalMeshComponent is not valid!"), ANSI_TO_TCHAR(__FUNCTION__));
		return;
	}
	
	SkeletalMesh->SetAllBodiesBelowSimulatePhysics(CurrentAnimationProperties->BoneName, false, CurrentAnimationProperties->bIncludeBone);
	SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

bool UST_PhysicalAnimationComponent::HasActivePhysicalReaction() const
{
	const bool bHasNoReaction = CurrentReactionType == EST_PhysicalReactionType::None && !PhysicalReactionPropertiesMap.Contains(CurrentReactionType);
	return !bHasNoReaction;
}
