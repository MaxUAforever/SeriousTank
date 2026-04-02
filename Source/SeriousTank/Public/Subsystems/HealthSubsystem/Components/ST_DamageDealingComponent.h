#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ST_DamageDealingComponent.generated.h"

class AController;

// Struct containing necessary information for calculating and applying damage to a damage receiver.
struct FST_DamageDealingParameters
{
	const UPrimitiveComponent* ImpactedComponent;
	FVector DealingLocation;
	FVector DamageDealerDirection;
	FVector ImpactNormal;
};

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
