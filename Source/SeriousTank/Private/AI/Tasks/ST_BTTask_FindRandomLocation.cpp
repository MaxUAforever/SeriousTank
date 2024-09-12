#include "AI/Tasks/ST_BTTask_FindRandomLocation.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

UST_BTTask_FindRandomLocation::UST_BTTask_FindRandomLocation(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "Find random point";
}

EBTNodeResult::Type UST_BTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller)
	{
		return EBTNodeResult::Failed;
	}

	APawn* Pawn = Controller->GetPawn();
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (NavSys == nullptr || Pawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	FNavLocation ResultLocation;
	NavSys->GetRandomReachablePointInRadius(Pawn->GetActorLocation(), Radius, ResultLocation);
	
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), ResultLocation.Location);
	return EBTNodeResult::Succeeded;
}