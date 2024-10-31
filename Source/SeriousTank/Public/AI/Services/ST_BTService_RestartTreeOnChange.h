#pragma once

#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BehaviorTree/Blackboard/BlackboardKeyEnums.h"
#include "ST_BTService_RestartTreeOnChange.generated.h"

/**
 * BehaviorTree Service that allows to track the changing of value of bool or object variable 
 * in blackboard and restart it in case if expected BasicOperation for it matches the actual result.
 */
UCLASS()
class SERIOUSTANK_API UST_BTService_RestartTreeOnChange : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
protected:
	UST_BTService_RestartTreeOnChange(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	EBlackboardNotificationResult OnBlackboardKeyValueChange(const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID);

protected:
	UPROPERTY(Category = Blackboard, EditAnywhere, meta = (DisplayName = "Key Query"))
	TEnumAsByte<EBasicKeyOperation::Type> BasicOperation;
};
