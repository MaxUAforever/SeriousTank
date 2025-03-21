#include "Subsystems/HealthSubsystem/ST_HealthComponent.h"

#include "Engine/World.h"
#include "Subsystems/HealthSubsystem/ST_HealthSubsystem.h"

void UST_HealthComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UST_HealthSubsystem* HealthSubsystem = GetWorld()->GetSubsystem<UST_HealthSubsystem>())
	{
		HealthSubsystem->RegisterHealthActor(GetOwner());
	}

	CurrentHealthValue = MaxHealthvalue;
}

void UST_HealthComponent::AddHealthValue(float DeltaHealthValue)
{
	const float OldHealthValue = CurrentHealthValue;
	CurrentHealthValue = FMath::Clamp(CurrentHealthValue + DeltaHealthValue, 0.f, MaxHealthvalue);

	EHealthChangingType HealthChangingType = CurrentHealthValue < OldHealthValue ? EHealthChangingType::Damage : EHealthChangingType::Healing;
	OnHealthValueChangedDelegate.Broadcast(CurrentHealthValue, HealthChangingType);

	if (FMath::IsNearlyZero(CurrentHealthValue))
	{
		OnOwnerKilledDelegate.Broadcast();
	}
}

void UST_HealthComponent::SetMaxHealthValue(float InMaxHealth)
{
	MaxHealthvalue = InMaxHealth;
	OnMaxHealthValueChangedDelegate.Broadcast(MaxHealthvalue);
}
