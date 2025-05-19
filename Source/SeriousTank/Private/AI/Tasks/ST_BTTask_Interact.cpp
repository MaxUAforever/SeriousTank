#include "AI/Tasks/ST_BTTask_Interact.h"

#include "Subsystems/AIManagerSubsystem/ST_AIInteractionsManagerSubsystem.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PlayerInteractionSubsystem/Public/Actions/BaseInterruptibleAction.h"
#include "PlayerInteractionSubsystem/Public/Components/InteractingComponent.h"
#include "PlayerInteractionSubsystem/Public/PlayerInteractionSubsystem.h"

UST_BTTask_Interact::UST_BTTask_Interact(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "Interact";
}

EBTNodeResult::Type UST_BTTask_Interact::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return EBTNodeResult::Failed;
	}

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

	UInteractingComponent* InteractingComponent = Pawn->GetComponentByClass<UInteractingComponent>();
	if (!IsValid(InteractingComponent) || InteractingComponent->IsInteracting())
	{
		return EBTNodeResult::Failed;
	}

	const bool bDeactivatingInteractionStarted = InteractingComponent->StartInteraction();
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(BlackboardKey.SelectedKeyName, bDeactivatingInteractionStarted);

	UPlayerInteractionSubsystem* PlayerInteractionSubsystem = World->GetSubsystem<UPlayerInteractionSubsystem>();
	if (IsValid(PlayerInteractionSubsystem))
	{
		const UBaseInterruptibleAction* InterruptibleAction = Cast<UBaseInterruptibleAction>(PlayerInteractionSubsystem->GetCurrentInterractionAction(InteractingComponent));

		OwnerComp.GetBlackboardComponent()->SetValueAsFloat(BBInteractionActivationTimeKey, IsValid(InterruptibleAction) ? InterruptibleAction->GetTotalActivationTime() : 0.f);
	}

	return EBTNodeResult::Succeeded;
}