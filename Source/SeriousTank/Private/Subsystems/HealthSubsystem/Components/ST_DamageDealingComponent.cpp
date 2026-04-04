#include "Subsystems/HealthSubsystem/Components/ST_DamageDealingComponent.h"

#include "Core/Bus/ST_Bus.h"
#include "Components/Armor/ST_ArmorComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "Subsystems/HealthSubsystem/ST_HealthSubsystem.h"

bool UST_DamageDealingComponent::TryToDealDamage(AController* DamageDealer, AActor* DamageReciever, const FST_DamageDealingParameters& DamageDealingInfo)
{
    if (!IsValid(DamageReciever) || !IsValid(DamageDealer))
    {
		UE_LOG(LogTemp, Warning, TEXT("%s: DamageReciever or DamageDealer is invalid"), ANSI_TO_TCHAR(__FUNCTION__));
        return false;
    }

	UST_HealthSubsystem* HealthSubsystem = GetWorld()->GetSubsystem<UST_HealthSubsystem>();
	if (!IsValid(HealthSubsystem))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: HealthSubsystem is not valid"), ANSI_TO_TCHAR(__FUNCTION__));
		return false;
	}

	CurrentDamageValue = DamageValue;

	const bool bCanPenetrateArmor = TryToPenetrateArmor(DamageReciever, DamageDealingInfo);
	
	FST_DamageDealingEventBus::Broadcast(&FST_HealthSubsystemEvents::OnDamageDealingAttempt, DamageDealer, DamageReciever, DamageDealingInfo, bCanPenetrateArmor ? CurrentDamageValue : 0.f);
	if (!bCanPenetrateArmor)
	{
		return false;
	}

	return HealthSubsystem->DealDamage(DamageDealer, DamageReciever, CurrentDamageValue);
}

bool UST_DamageDealingComponent::TryToPenetrateArmor(const AActor* DamageReciever, const FST_DamageDealingParameters& DamageDealingInfo)
{
	const UST_ArmorComponent* ArmorComponent = DamageReciever->FindComponentByClass<UST_ArmorComponent>();
	if (!IsValid(ArmorComponent))
	{
		return true;
	}

	const FST_ArmorSectionData* ArmorSectionData = ArmorComponent->FindArmorForComponent(DamageDealingInfo.ImpactedComponent);
	if (!ArmorSectionData)
	{
		return true;
	}
	
	float EffectiveArmor = ArmorSectionData->ArmorValue;
	if (EffectiveArmor <= KINDA_SMALL_NUMBER)
	{
		return true;
	}

	if (ArmorSectionData->bAffectsByHitAngle)
	{
		const FVector ImpactDir = DamageDealingInfo.DamageDealerDirection.GetSafeNormal();
		const FVector ImpactNormal = DamageDealingInfo.ImpactNormal.GetSafeNormal();
		float CosTheta = FVector::DotProduct(-ImpactDir, ImpactNormal);

		constexpr float RicochetThreshold = 0.2f;
		if (CosTheta < RicochetThreshold)
		{
			// No penetration if hit angle is too shallow (>= ~78 degrees)
			return false;
		}

		// Limit max armor multiplier (~5x)
		CosTheta = FMath::Clamp(CosTheta, 0.2f, 1.0f);

		EffectiveArmor /= CosTheta;
	}

	// Define thresholds for penetration. These can be tweaked for game balance.
	constexpr float MinPenetrationThreshold = 1.0f;
	constexpr float FullPenetrationThreshold = 1.5f;

	const float PenetrationRatio = ArmorPenetrationValue / EffectiveArmor;

	// If penetration ratio is below the minimum threshold, no damage is dealt. 
	// If it's above the full penetration threshold, full damage is dealt. In between, damage scales linearly.
	const float DamageMultiplier = FMath::Clamp((PenetrationRatio - MinPenetrationThreshold) / (FullPenetrationThreshold - MinPenetrationThreshold), 0.0f, 1.0f);
	
	CurrentDamageValue *= DamageMultiplier;

	return PenetrationRatio >= MinPenetrationThreshold;
}
