#pragma once

#include "Components/ActorComponent.h"
#include "GameplayAbilitySpecHandle.h"
#include "Math/NumericLimits.h"
#include "ST_EquipmentManagerComponent.generated.h"

class AST_EquippableAbilityItem;
class UST_VehicleAbilitySystemComponent;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnItemEquippedDelegate, const AST_EquippableAbilityItem*, int32);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SERIOUSTANK_API UST_EquipmentManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FOnItemEquippedDelegate OnItemEquipped;

private:
	TArray<AST_EquippableAbilityItem*> EquippedItems;
	int32 MaxItemsAmount;

public:	
	UST_EquipmentManagerComponent();

	FORCEINLINE int32 GetMaxItemsAmount() const { return MaxItemsAmount; }
	void SetMaxItemsAmount(int32 InMaxItemsAmount);

	void GrantEquipmentAbilities(AST_EquippableAbilityItem* AbilityItem);
	void GrantEquipmentAbilitiesByIndex(AST_EquippableAbilityItem* AbilityItem, int32 Index);

	void RemoveEquipment(AST_EquippableAbilityItem* AbilityItem);
	void RemoveEquipmentByIndex(int32 Index);

	bool ActivateAbility(AST_EquippableAbilityItem* AbilityItem) const;
	bool ActivateAbilityByIndex(int32 Index) const;
	
private:
	UST_VehicleAbilitySystemComponent* GetAbilitySystemComponent() const;
};
