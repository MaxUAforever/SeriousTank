// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "ST_BTTask_Interact.generated.h"

UCLASS()
class SERIOUSTANK_API UST_BTTask_Interact : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	explicit UST_BTTask_Interact(const FObjectInitializer& ObjectInitializer);
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FName BBInteractionActivationTimeKey = FName("InteractionActivationTime");
};
