#include "AI/Tasks/ST_BTTask_AttackTarget.h"

#include "Actors/Weapons/ST_BaseWeapon.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/Weapons/ST_BaseWeaponsManagerComponent.h"

UST_BTTask_AttackTarget::UST_BTTask_AttackTarget(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "Attack Target";
	bTickIntervals = true;
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

	if (!CheckAvailableWeapon(WeaponComponent))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBCanAttackKey, false);
		return EBTNodeResult::Succeeded;
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

void UST_BTTask_AttackTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FBTAttackTargetTaskMemory* CurrentMemory = CastInstanceNodeMemory<FBTAttackTargetTaskMemory>(NodeMemory);
	if (!IsValid(CurrentMemory->OwnerWeaponsManagerComponent))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}
		
	const bool bCanShoot = CheckAvailableWeapon(CurrentMemory->OwnerWeaponsManagerComponent);
	const bool bHasTarget = OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey()) != nullptr;

	if (!bCanShoot || !bHasTarget)
	{
		if (AAIController* AIController = OwnerComp.GetAIOwner())
		{
			AIController->ClearFocus(EAIFocusPriority::Gameplay);
			CurrentMemory->OwnerWeaponsManagerComponent->StopFire();

			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
}

bool UST_BTTask_AttackTarget::CheckAvailableWeapon(UST_BaseWeaponsManagerComponent* WeaponComponent)
{
	AST_BaseWeapon* CurrentWeapon = WeaponComponent->GetCurrentWeapon();
	if (IsValid(CurrentWeapon) && CurrentWeapon->GetTotalAmmoCount() > 0)
	{
		if (!CurrentWeapon->IsFireForced())
		{
			WeaponComponent->StartFire();
		}

		return true;
	}

	if (WeaponComponent->GetWeapons().Num() == 0)
	{
		return false;
	}

	int32 WeaponIndex = 0;
	for (AST_BaseWeapon* NewWeapon : WeaponComponent->GetWeapons())
	{
		if (IsValid(NewWeapon) && NewWeapon->GetTotalAmmoCount() > 0)
		{
			WeaponComponent->SwitchWeapon(WeaponIndex);
			WeaponComponent->StartFire();

			return true;
		}

		++WeaponIndex;
	}

	return false;
}