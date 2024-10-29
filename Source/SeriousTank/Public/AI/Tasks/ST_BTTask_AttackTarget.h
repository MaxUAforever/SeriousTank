#pragma once

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "ST_BTTask_AttackTarget.generated.h"

class UST_BaseWeaponsManagerComponent;

struct FBTAttackTargetTaskMemory
{
	UST_BaseWeaponsManagerComponent* OwnerWeaponsManagerComponent;
};

/**
 * 
 */
UCLASS()
class SERIOUSTANK_API UST_BTTask_AttackTarget : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	explicit UST_BTTask_AttackTarget(const FObjectInitializer& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	virtual uint16 GetInstanceMemorySize() const override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	bool CheckAvailableWeapon(UST_BaseWeaponsManagerComponent* WeaponComponent);

protected:
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FName BBCanAttackKey;
};
