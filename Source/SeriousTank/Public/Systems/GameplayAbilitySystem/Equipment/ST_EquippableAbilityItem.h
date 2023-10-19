#pragma once

#include "GameFramework/Actor.h"
#include "GameplayAbilitySpecHandle.h"
#include "Systems/GameplayAbilitySystem/ST_AbilitySet.h"
#include "ST_EquippableAbilityItem.generated.h"

struct FGameplayAbilitySpec;
struct FGameplayAbilitySpecHandle;

UCLASS()
class SERIOUSTANK_API AST_EquippableAbilityItem : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	FST_AbilitySet EquipmentAbilitySetToGrant;

private:
	TUniquePtr<FGameplayAbilitySpecHandle> AbilitySpecHandle;

public:
	virtual void AttachToParentActor(AActor* ParentActor, USceneComponent* ParentActorComponent = nullptr);

	FORCEINLINE const FST_AbilitySet& GetAbilitySet() const { return EquipmentAbilitySetToGrant; }
	TOptional<FGameplayAbilitySpecHandle> GetAbilityHandle() const;
	
	virtual void OnGiveAbility(const FGameplayAbilitySpec& AbilitySpec);
	virtual void OnRemoveAbility(const FGameplayAbilitySpec& AbilitySpec);

	virtual bool CheckCost() { return true; };
	virtual void ApplyCost() {};
	
	virtual void HandleAbilityActivated() {};
	virtual void HandleAbilityEnded() {};

private:
	UFUNCTION()
	void OnParentDestroyed(AActor* DestroyedOwnerActor);
};
