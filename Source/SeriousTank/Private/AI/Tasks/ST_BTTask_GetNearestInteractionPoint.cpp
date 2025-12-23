#include "AI/Tasks/ST_BTTask_GetNearestInteractionPoint.h"

#include "Subsystems/AIManagerSubsystem/ST_AIInteractionsManagerSubsystem.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UST_BTTask_GetNearestInteractionPoint::UST_BTTask_GetNearestInteractionPoint(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "Get Interaction Point";
}

EBTNodeResult::Type UST_BTTask_GetNearestInteractionPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!IsValid(Controller) || !IsValid(OwnerComp.GetBlackboardComponent()))
	{
		return EBTNodeResult::Failed;
	}

	APawn* Pawn = Controller->GetPawn();
	if (!IsValid(Pawn))
	{
		return EBTNodeResult::Failed;
	}

	const FVector PawnLocation = Pawn->GetActorLocation();
	const AActor* NearestInteractionActor = nullptr;
	float MinDistance = TNumericLimits<float>::Max();

	for (EAIInteractionType InteractionType : InteractionTypes)
	{
		const AActor* NewInteractionActor = FindNearestInteractionActor(Pawn, InteractionType);
		if (NewInteractionActor == nullptr)
		{
			continue;
		}

		const float Distance = FVector::DistSquared(PawnLocation, NewInteractionActor->GetActorLocation());
		if (Distance < MinDistance)
		{
			NearestInteractionActor = NewInteractionActor;
			MinDistance = Distance;
		}
	}

	if (IsValid(NearestInteractionActor))
	{
		const FVector& InteractionActorLocation = NearestInteractionActor->GetActorLocation();
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName, InteractionActorLocation);
	
		if (Controller->GetFocalPoint() != InteractionActorLocation)
		{
			Controller->SetFocalPoint(InteractionActorLocation);
		}
	}

	return IsValid(NearestInteractionActor) ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}

const AActor* UST_BTTask_GetNearestInteractionPoint::FindNearestInteractionActor(const APawn* OwnerPawn, EAIInteractionType InteractionType) const
{
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return nullptr;
	}

	const UST_AIInteractionsManagerSubsystem* AIManagerSubsystem = World->GetSubsystem<UST_AIInteractionsManagerSubsystem>();
	if (!IsValid(AIManagerSubsystem))
	{
		return nullptr;
	}

	const TSet<TObjectPtr<const AActor>>* InteractionActors = AIManagerSubsystem->GetInteractionActors(InteractionType);
	if (InteractionActors == nullptr)
	{
		return nullptr;
	}

	const FVector PawnLocation = OwnerPawn->GetActorLocation();
	const AActor* NearestInteractionActor = nullptr;
	float MinDistance = TNumericLimits<float>::Max();

	for (const AActor* InteractionActor : *InteractionActors)
	{
		if (!IsValid(InteractionActor))
		{
			continue;
		}

		const float Distance = FVector::DistSquared(PawnLocation, InteractionActor->GetActorLocation());
		if (Distance < MinDistance)
		{
			NearestInteractionActor = InteractionActor;
			MinDistance = Distance;
		}
	}

	return NearestInteractionActor;
}