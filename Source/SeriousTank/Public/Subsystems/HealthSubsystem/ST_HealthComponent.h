#pragma once

#include "Components/ActorComponent.h"
#include "ST_HealthComponent.generated.h"

enum class EHealthChangingType : uint8
{
	Damage,
	Healing
};

DECLARE_MULTICAST_DELEGATE(FOnOwnerKilledDelegate)
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHealthValueChangedDelegate, float /*CurrentHealthValue*/, EHealthChangingType /*HealthChangingType*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMaxHealthValueChangedDelegate, float /*MaxHealthValue*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SERIOUSTANK_API UST_HealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FOnOwnerKilledDelegate OnOwnerKilledDelegate;
	FOnHealthValueChangedDelegate OnHealthValueChangedDelegate;
	FOnMaxHealthValueChangedDelegate OnMaxHealthValueChangedDelegate;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0.1"));
	float MaxHealthvalue = 100.f;

private:
	float CurrentHealthValue;

public:
	float GetCurrentHealth() const { return CurrentHealthValue; }
	void AddHealthValue(float DeltaHealthValue);

	float GetMaxHealth() const { return MaxHealthvalue; }
	void SetMaxHealthValue(float InMaxHealth);

protected:
	virtual void BeginPlay() override;		
};
