#pragma once

#include "Components/ActorComponent.h"
#include "ST_HealthComponent.generated.h"

// TODO: In the future there should be different signatures of this events, so they are splitted.
DECLARE_MULTICAST_DELEGATE_OneParam(FOnDamageDealed, float /*CurrentHealthValue*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealed, float /*CurrentHealthValue*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SERIOUSTANK_API UST_HealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FOnDamageDealed OnDamageDealedDelegate;
	FOnHealed OnHealedDelegate;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0.1"));
	float MaxHealthvalue = 100.f;

private:
	float CurrentHealthValue;

public:	
	void AddHealthValue(float Value);

protected:
	virtual void BeginPlay() override;		
};
