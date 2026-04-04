#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Subsystems/HealthSubsystem/ST_HealthSubsystemTypes.h"
#include "ST_DamageDealingComponent.generated.h"

class AController;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SERIOUSTANK_API UST_DamageDealingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	float GetDamageValue() const { return DamageValue; };
	void SetDamageValue(float InDamageValue) { DamageValue = InDamageValue; };

	bool TryToDealDamage(AController* DamageDealer, AActor* DamageReciever, const FST_DamageDealingParameters& DamageDealingInfo);

private:
	bool TryToPenetrateArmor(const AActor* DamageReciever, const FST_DamageDealingParameters& DamageDealingInfo);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DamageValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ArmorPenetrationValue;

private:
	// This value is used to store the damage value that will actually be applied 
	// to the health subsystem after armor penetration is calculated.
	float CurrentDamageValue;
};
