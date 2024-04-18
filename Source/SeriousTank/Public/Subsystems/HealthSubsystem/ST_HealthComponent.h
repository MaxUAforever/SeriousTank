#pragma once

#include "Components/ActorComponent.h"
#include "ST_HealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthValueChanged, float);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SERIOUSTANK_API UST_HealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FOnHealthValueChanged OnHealthValueChanged;

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
