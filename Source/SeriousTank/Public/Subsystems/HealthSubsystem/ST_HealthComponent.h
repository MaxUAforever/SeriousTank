#pragma once

#include "Components/ActorComponent.h"
#include "ST_HealthComponent.generated.h"

enum class EHealthChangingType : uint8
{
	Damage,
	Healing
};

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHealthValueChanged, float /*CurrentHealthValue*/, EHealthChangingType /*HealthChangingType*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SERIOUSTANK_API UST_HealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FOnHealthValueChanged OnHealthValueChangedDelegate;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0.1"));
	float MaxHealthvalue = 100.f;

private:
	float CurrentHealthValue;

public:	
	float GetCurrentHealth() { return CurrentHealthValue; }
	void AddHealthValue(float Value);

protected:
	virtual void BeginPlay() override;		
};
