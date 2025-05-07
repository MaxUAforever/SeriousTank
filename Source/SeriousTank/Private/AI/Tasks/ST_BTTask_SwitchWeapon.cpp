#include "AI/Tasks/ST_BTTask_SwitchWeapon.h"

#include "Actors/Weapons/ST_BaseWeapon.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/Weapons/ST_BaseWeaponsManagerComponent.h"

UST_BTTask_SwitchWeapon::UST_BTTask_SwitchWeapon(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "Switch Weapon";
}

EBTNodeResult::Type UST_BTTask_SwitchWeapon::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return EBTNodeResult::Failed;
	}

	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!IsValid(Controller))
	{
		return EBTNodeResult::Failed;
	}

	APawn* Pawn = Controller->GetPawn();
	if (!IsValid(Pawn))
	{
		return EBTNodeResult::Failed;
	}

	UST_BaseWeaponsManagerComponent* WeaponComponent = Pawn->GetComponentByClass<UST_BaseWeaponsManagerComponent>();
	if (!IsValid(WeaponComponent))
	{
		return EBTNodeResult::Failed;
	}

	int32 WeaponIndex = 0;
	for (AST_BaseWeapon* NewWeapon : WeaponComponent->GetWeapons())
	{
		if (IsValid(NewWeapon) && NewWeapon->GetTotalAmmoCount() > 0)
		{
			WeaponComponent->StartSwitchingWeapon(WeaponIndex);
			return EBTNodeResult::Succeeded;
		}

		++WeaponIndex;
	}

	return EBTNodeResult::Failed;
}