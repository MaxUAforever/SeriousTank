#pragma once

#include "Components/ActorComponent.h"
#include "ST_DamageDealingComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SERIOUSTANK_API UST_DamageDealingComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DamageValue;

public:
	float GetDamageValue() const { return DamageValue; };
	void SetDamageValue(float InDamageValue) { DamageValue = InDamageValue; };

	void StartDealingDamage(AActor* ContactActor, const FVector& DealingLocation);	
};
