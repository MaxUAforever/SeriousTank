#pragma once

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_GetNextPatrolPoint.generated.h"

class UAIPatrollingComponent;

/**
 * 
 */
UCLASS()
class AIPATROLLINGSUBSYSTEM_API UBTTask_GetNextPatrolPoint : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	explicit UBTTask_GetNextPatrolPoint(const FObjectInitializer& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	void UpdatePatrollingComponent(UAIPatrollingComponent* PatrollingComponent, int32 NextPointIndex);

protected:
	UPROPERTY(EditAnywhere, Category = "Settings");
	float AcceptableDistance = 50.f;
};
