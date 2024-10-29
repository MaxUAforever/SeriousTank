#include "AI/Tasks/ST_BTTask_GetOppositeLocation.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"

UST_BTTask_GetOppoiteLocation::UST_BTTask_GetOppoiteLocation(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "Get Opposite Direction";
}

void UST_BTTask_GetOppoiteLocation::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (UBlackboardData* BBAsset = GetBlackboardAsset())
	{
		BBOppositeLocationKey.ResolveSelectedKey(*BBAsset);
	}
}

EBTNodeResult::Type UST_BTTask_GetOppoiteLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller || !OwnerComp.GetBlackboardComponent())
	{
		return EBTNodeResult::Failed;
	}

	APawn* Pawn = Controller->GetPawn();
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (NavSys == nullptr || Pawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey()));
	if (!TargetActor)
	{
		return EBTNodeResult::Failed;
	}

	FVector OppositeDirection = Pawn->GetActorLocation() - TargetActor->GetActorLocation();
	OppositeDirection.Normalize();

	const FVector TargetLocation = Pawn->GetActorLocation() + (OppositeDirection * Distance);

	FNavLocation ResultLocation;
	NavSys->GetRandomReachablePointInRadius(TargetLocation, AcceptableRadius, ResultLocation);
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBOppositeLocationKey.SelectedKeyName, ResultLocation.Location);
	
	if (Controller->GetFocalPoint() != ResultLocation.Location)
	{
		Controller->SetFocalPoint(ResultLocation.Location);
	}

	return EBTNodeResult::Succeeded;
}