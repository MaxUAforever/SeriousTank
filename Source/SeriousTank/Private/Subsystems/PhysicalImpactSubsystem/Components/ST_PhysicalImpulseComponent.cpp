#include "Subsystems/PhysicalImpactSubsystem/Components/ST_PhysicalImpulseComponent.h"

// Game includes
#include "Subsystems/PhysicalImpactSubsystem/ST_PhysicalImpactSubsystem.h"
#include "Subsystems/PhysicalImpactSubsystem/ST_PhysicalImpactTypes.h"

// Engine includes
#include "Engine/World.h"

void UST_PhysicalImpulseComponent::ApplyPhysicalImpulse(AActor* ImpactedActor, const FST_PhysicalImpactParameters& ImpactParameters)
{
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogPhysicalImpact, Warning, TEXT("%s: Failed to get a valid world!"), ANSI_TO_TCHAR(__FUNCTION__));
		return;
	};

	UST_PhysicalImpactSubsystem* PhysicalImpactSubsystem = World->GetSubsystem<UST_PhysicalImpactSubsystem>();
	if (!IsValid(PhysicalImpactSubsystem))
	{
		UE_LOG(LogPhysicalImpact, Warning, TEXT("%s: Failed to get PhysicalImpactSubsystem!"), ANSI_TO_TCHAR(__FUNCTION__));
		return;
	}

	PhysicalImpactSubsystem->ApplyPhysicalImpact(this, ImpactedActor, ImpactParameters);
}