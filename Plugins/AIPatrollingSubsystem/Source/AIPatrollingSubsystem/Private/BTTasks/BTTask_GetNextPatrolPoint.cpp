#include "BTTasks/BTTask_GetNextPatrolPoint.h"

#include "Actors/AIPatrolPath.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/AIPatrollingComponent.h"

UBTTask_GetNextPatrolPoint::UBTTask_GetNextPatrolPoint(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "Get next patrol point";

	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_GetNextPatrolPoint, BlackboardKey));
}

EBTNodeResult::Type UBTTask_GetNextPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	APawn* Pawn = IsValid(Controller) ? Controller->GetPawn() : nullptr;
	if (!IsValid(Pawn) || !OwnerComp.GetBlackboardComponent())
	{
		return EBTNodeResult::Failed;
	}

	UAIPatrollingComponent* PatrollingComponent = Controller->GetComponentByClass<UAIPatrollingComponent>();
	if (!PatrollingComponent)
	{
		return EBTNodeResult::Failed;
	}

	const AAIPatrolPath* PatrolPath = PatrollingComponent->GetCurrentPatrolPath();
	if (!IsValid(PatrolPath))
	{
		return EBTNodeResult::Failed;
	}
	
	const int32 CurrentPointIndex = PatrollingComponent->GetCurrentPathPointIndex();
	const FVector& CurrentPointLocation = PatrolPath->GetPointLocation(CurrentPointIndex);
	
	FVector TargetDirection;
	if (FVector::Distance(Pawn->GetActorLocation(), CurrentPointLocation) > AcceptableDistance)
	{
		TargetDirection = CurrentPointLocation;
	}
	else
	{
		const int32 NextPointIndex = PatrolPath->GetNextPatrolPoint(CurrentPointIndex, PatrollingComponent->GetCurrentDirection());
		TargetDirection = PatrolPath->GetPointLocation(NextPointIndex);

		UpdatePatrollingComponent(PatrollingComponent, NextPointIndex);
	}

	if (Controller->GetFocalPoint() != TargetDirection)
	{
		Controller->SetFocalPoint(TargetDirection);
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName, TargetDirection);
	return EBTNodeResult::Succeeded;
}

void UBTTask_GetNextPatrolPoint::UpdatePatrollingComponent(UAIPatrollingComponent* PatrollingComponent, int32 NextPointIndex)
{
	const AAIPatrolPath* PatrolPath = PatrollingComponent->GetCurrentPatrolPath();
	if (!IsValid(PatrolPath))
	{
		return;
	}

	const int32 CurrentPointIndex = PatrollingComponent->GetCurrentPathPointIndex();

	const bool bIsTwoWayPath = PatrolPath->GetPatrolType() == EPatrolType::TwoWay;

	const bool bIsMovingForward = PatrollingComponent->GetCurrentDirection() == EDirectionType::Forward;
	const bool bIsOnLastPoint = bIsMovingForward && CurrentPointIndex == PatrolPath->GetPathPoints().Num() - 1;
	const bool bIsOnFirstPoint = !bIsMovingForward && CurrentPointIndex == 0;

	if (bIsTwoWayPath && (bIsOnLastPoint || bIsOnFirstPoint))
	{
		PatrollingComponent->SetCurrentDirection(bIsMovingForward ? EDirectionType::Backward : EDirectionType::Forward);
	}

	PatrollingComponent->SetCurrentPathPointIndex(NextPointIndex);
}
