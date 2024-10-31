#include "AI/Services/ST_BTService_RestartTreeOnChange.h"

#include "BehaviorTree/BlackboardComponent.h"

UST_BTService_RestartTreeOnChange::UST_BTService_RestartTreeOnChange(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "Restart on change";

	bTickIntervals = false;
	INIT_SERVICE_NODE_NOTIFY_FLAGS();

	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UST_BTService_RestartTreeOnChange, BlackboardKey), AActor::StaticClass());
	BlackboardKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UST_BTService_RestartTreeOnChange, BlackboardKey));
}

void UST_BTService_RestartTreeOnChange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard)
	{
		return;
	}
		
	const FBlackboard::FKey KeyID = BlackboardKey.GetSelectedKeyID();
	if (OnBlackboardKeyValueChange(*Blackboard, KeyID) == EBlackboardNotificationResult::ContinueObserving)
	{
		Blackboard->RegisterObserver(KeyID, this, FOnBlackboardChangeNotification::CreateUObject(this, &ThisClass::OnBlackboardKeyValueChange));
	}
}

void UST_BTService_RestartTreeOnChange::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnCeaseRelevant(OwnerComp, NodeMemory);

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (Blackboard)
	{
		Blackboard->UnregisterObserversFrom(this);
	}
}

EBlackboardNotificationResult UST_BTService_RestartTreeOnChange::OnBlackboardKeyValueChange(const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID)
{
	UBlackboardKeyType* KeyCDO = BlackboardKey.SelectedKeyType->GetDefaultObject<UBlackboardKeyType>();
	const uint8* KeyMemory = Blackboard.GetKeyRawData(BlackboardKey.GetSelectedKeyID());
	
	bool bResult = false;
	if (KeyCDO && KeyMemory)
	{
		bResult = KeyCDO->WrappedTestBasicOperation(Blackboard, KeyMemory, (EBasicKeyOperation::Type)BasicOperation.GetIntValue());
	}

	if (bResult)
	{
		UBehaviorTreeComponent* OwnerComp = Cast<UBehaviorTreeComponent>(Blackboard.GetBrainComponent());
		if (OwnerComp)
		{
			OwnerComp->RestartTree();
		}
	}

	return bResult ? EBlackboardNotificationResult::RemoveObserver : EBlackboardNotificationResult::ContinueObserving;
}
