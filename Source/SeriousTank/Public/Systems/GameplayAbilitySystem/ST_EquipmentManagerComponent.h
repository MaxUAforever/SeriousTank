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
	UPROPERTY()
	TArray<AST_EquippableAbilityItem*> EquippedItems;

	int32 MaxItemsAmount;

public:	
	UST_EquipmentManagerComponent();

public:
	FORCEINLINE int32 GetMaxItemsAmount() const { return MaxItemsAmount; }
	void SetMaxItemsAmount(int32 InMaxItemsAmount);

	void EquipItem(AST_EquippableAbilityItem* AbilityItem, USceneComponent* ParentComponent = nullptr);
	void EquipItemByIndex(AST_EquippableAbilityItem* AbilityItem, int32 Index, USceneComponent* ParentComponent = nullptr);
	void EquipItemByIndex(TSubclassOf<AST_EquippableAbilityItem> AbilityItemClass, int32 Index, USceneComponent* ParentComponent = nullptr);

	void RemoveEquipment(AST_EquippableAbilityItem* AbilityItem);
	void RemoveEquipmentByIndex(int32 Index);

	bool ActivateAbility(AST_EquippableAbilityItem* AbilityItem) const;
	bool ActivateAbilityByIndex(int32 Index) const;

private:
	bool GrantEquipmentAbilitiesByIndex(AST_EquippableAbilityItem* AbilityItem, int32 Index);

	UST_VehicleAbilitySystemComponent* GetAbilitySystemComponent() const;
};
