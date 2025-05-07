#include "AI/Tasks/ST_BTTask_AttackTarget.h"

#include "Actors/Weapons/ST_BaseWeapon.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/Weapons/ST_BaseWeaponsManagerComponent.h"

UST_BTTask_AttackTarget::UST_BTTask_AttackTarget(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "Attack Target";
	bTickIntervals = false;
	INIT_TASK_NODE_NOTIFY_FLAGS();
}

EBTNodeResult::Type UST_BTTask_AttackTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!IsValid(AIController))
	{
		return EBTNodeResult::Failed;
	}

	APawn* AIPawn = AIController->GetPawn();
	if (!IsValid(AIPawn))
	{
		return EBTNodeResult::Failed;
	}

	const UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	UST_BaseWeaponsManagerComponent* WeaponComponent = AIPawn->GetComponentByClass<UST_BaseWeaponsManagerComponent>();
	if (!IsValid(WeaponComponent) || !IsValid(Blackboard))
	{
		return EBTNodeResult::Failed;
	}

	FBTAttackTargetTaskMemory* CurrentMemory = CastInstanceNodeMemory<FBTAttackTargetTaskMemory>(NodeMemory);
	CurrentMemory->OwnerWeaponsManagerComponent = WeaponComponent;

	AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(GetSelectedBlackboardKey()));
	if (AIController->GetFocusActor() != TargetActor)
	{
		AIController->SetFocus(TargetActor);
	}

	WeaponComponent->StartFire();
	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UST_BTTask_AttackTarget::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FBTAttackTargetTaskMemory* CurrentMemory = CastInstanceNodeMemory<FBTAttackTargetTaskMemory>(NodeMemory);
	if (AAIController* AIController = OwnerComp.GetAIOwner())
	{
		AIController->ClearFocus(EAIFocusPriority::Gameplay);
		CurrentMemory->OwnerWeaponsManagerComponent->StopFire();
	}

	return EBTNodeResult::Aborted;
}

uint16 UST_BTTask_AttackTarget::GetInstanceMemorySize() const
{
	return sizeof(FBTAttackTargetTaskMemory);
}