#include "AI/Tasks/ST_BTTask_GetNextPatrolPoint.h"

UST_BTTask_GetNextPatrolPoint::UST_BTTask_GetNextPatrolPoint(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "Get Next Patrol Point";
}

EBTNodeResult::Type UST_BTTask_GetNextPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::Succeeded;
}

uint16 UST_BTTask_GetNextPatrolPoint::GetInstanceMemorySize() const
{
	return sizeof(FBTGetNextPatrolPointTaskMemory);
}
