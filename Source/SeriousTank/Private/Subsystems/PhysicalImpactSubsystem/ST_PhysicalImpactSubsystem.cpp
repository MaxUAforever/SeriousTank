#include "Subsystems/PhysicalImpactSubsystem/ST_PhysicalImpactSubsystem.h"

// Game includes
#include "Subsystems/PhysicalImpactSubsystem/Components/ST_PhysicalAnimationComponent.h"
#include "Subsystems/PhysicalImpactSubsystem/Components/ST_PhysicalImpulseComponent.h"

// Engine includes
#include "GameFramework/Actor.h"

void UST_PhysicalImpactSubsystem::ApplyPhysicalImpact(UST_PhysicalImpulseComponent* PhysicalImpulseComponent, AActor* ImpactedActor, const FST_PhysicalImpactParameters& ImpactParameters)
{
	if (!bIsPhysicalImpactEnabled)
	{
		return;
	}

	if (!IsValid(PhysicalImpulseComponent))
	{
		UE_LOG(LogPhysicalImpact, Warning, TEXT("%s: Invalid PhysicalImpulseComponent provided!"), ANSI_TO_TCHAR(__FUNCTION__));
		return;
	}

	if (!IsValid(ImpactedActor))
	{
		UE_LOG(LogPhysicalImpact, Warning, TEXT("%s: Invalid ImpactedActor provided!"), ANSI_TO_TCHAR(__FUNCTION__));
		return;
	}

	UST_PhysicalAnimationComponent* PhysicalAnimationComponent = ImpactedActor->GetComponentByClass<UST_PhysicalAnimationComponent>();
	if (!IsValid(PhysicalAnimationComponent))
	{
		return;
	}

	PhysicalAnimationComponent->ApplyPhysicalImpact(ImpactParameters, PhysicalImpulseComponent->GetImpulseStrength());
}
