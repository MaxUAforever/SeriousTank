#pragma once

#include "GameFramework/Actor.h"
#include "ST_EquippableAbilityItem.generated.h"

class UAttributeSet;
class UGameplayAbility;
class UGameplayEffect;
struct FGameplayAbilitySpec;
struct FGameplayAbilitySpecHandle;

USTRUCT(BlueprintType)
struct FEquipmentAbilitySet
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayAbility> GameplayAbilityClass;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UGameplayEffect>> GameplayEffectClasses;

	// TODO: Add AttributeSet for equipment properties
};


UCLASS()
class SERIOUSTANK_API AST_EquippableAbilityItem : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	FEquipmentAbilitySet EquipmentAbilitySetToGrant;

private:
	TUniquePtr<FGameplayAbilitySpecHandle> AbilitySpecHandle;

public:
	FORCEINLINE const FEquipmentAbilitySet& GetAbilitySet() const { return EquipmentAbilitySetToGrant; }

	virtual void OnGiveAbility(FGameplayAbilitySpec& AbilitySpec);
	virtual void OnRemoveAbility(FGameplayAbilitySpec& AbilitySpec);

	TOptional<FGameplayAbilitySpecHandle> GetAbilityHandle() const;
};
