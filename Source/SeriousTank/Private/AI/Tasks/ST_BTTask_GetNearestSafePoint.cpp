#include "AI/Tasks/ST_BTTask_GetNearestSafePoint.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"

UST_BTTask_GetNearestSafePoint::UST_BTTask_GetNearestSafePoint(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "Get Safe Point";
}

EBTNodeResult::Type UST_BTTask_GetNearestSafePoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	TArray<AActor*> AISafePlaces;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("AIBase"), AISafePlaces);
	if (AISafePlaces.Num() == 0)
	{
		return EBTNodeResult::Failed;
	}

	FVector PawnLocation = Pawn->GetActorLocation();
	float MinDistance = TNumericLimits<float>::Max();
	AActor* NearestSafePoint = nullptr;

	for (AActor* AISafePlace : AISafePlaces)
	{
		if (!IsValid(AISafePlace))
		{
			continue;
		}

		const float Distance = FVector::DistSquared(PawnLocation, AISafePlace->GetActorLocation());
		if (Distance < MinDistance)
		{
			NearestSafePoint = AISafePlace;
			MinDistance = Distance;
		}
	}

	if (IsValid(NearestSafePoint))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName, NearestSafePoint->GetActorLocation());
	
		if (Controller->GetFocusActor() != NearestSafePoint)
		{
			Controller->SetFocus(NearestSafePoint);
		}
	}

	return IsValid(NearestSafePoint) ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}