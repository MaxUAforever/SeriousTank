#include "AI/Tasks/ST_BTTask_ExitVehicle.h"

#include "Actors/Pawns/ST_BaseVehicle.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PlayerInteractionSubsystem/Public/Actions/BaseInterruptibleAction.h"
#include "PlayerInteractionSubsystem/Public/Components/InteractingComponent.h"
#include "PlayerInteractionSubsystem/Public/PlayerInteractionSubsystem.h"

UST_BTTask_ExitVehicle::UST_BTTask_ExitVehicle(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "Exit Vehicle";
}

EBTNodeResult::Type UST_BTTask_ExitVehicle::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!IsValid(Controller) || !IsValid(OwnerComp.GetBlackboardComponent()))
	{
		return EBTNodeResult::Failed;
	}

	AST_BaseVehicle* VehiclePawn = Cast<AST_BaseVehicle>(Controller->GetPawn());
	if (!IsValid(VehiclePawn))
	{
		return EBTNodeResult::Failed;
	}

	VehiclePawn->ExitVehicle();

	return EBTNodeResult::Succeeded;
}