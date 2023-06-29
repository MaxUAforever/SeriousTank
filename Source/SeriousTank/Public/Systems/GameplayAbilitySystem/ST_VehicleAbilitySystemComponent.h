#pragma once

#include "AbilitySystemComponent.h"
#include "ST_VehicleAbilitySystemComponent.generated.h"

class AST_EquippableAbilityItem;
struct FST_GameplayAbilityInfo;

UENUM(BlueprintType)
enum class EST_AbilityInputID : uint8
{
	// 0
	None				UMETA(DisplayName = "None"),
	// 1: LMB
	ConfirmAbility		UMETA(DisplayName = "ConfirmAbility"),
	// 2: RMB
	CancelAbility		UMETA(DisplayName = "CancelAbility"),
	// 3: 1
	EquippedItem1		UMETA(DisplayName = "EquippedItem1"),
	// 4: 2
	EquippedItem2		UMETA(DisplayName = "EquippedItem2"),
	// 5: 3
	EquippedItem3		UMETA(DisplayName = "EquippedItem3"),
	// 6: E
	Ability1			UMETA(DisplayName = "Ability1"),
	// 7: R
	Ability2			UMETA(DisplayName = "Ability2"),
	// 8: Left shift
	SpeedUp				UMETA(DisplayName = "SpeedUp"),
};

UCLASS()
class SERIOUSTANK_API UST_VehicleAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	FGameplayAbilitySpecHandle GiveAbility(const FGameplayAbilitySpec& AbilitySpec);
	FGameplayAbilitySpecHandle GiveAbility(const FST_GameplayAbilityInfo& AbilityInfo, UObject* SourceObject = nullptr, EST_AbilityInputID InputID = EST_AbilityInputID::None);

protected:
	virtual void OnGiveAbility(FGameplayAbilitySpec& AbilitySpec) override;
	virtual void OnRemoveAbility(FGameplayAbilitySpec& AbilitySpec) override;
};
