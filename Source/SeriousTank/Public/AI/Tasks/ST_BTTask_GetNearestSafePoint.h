// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "ST_BTTask_GetNearestSafePoint.generated.h"

/**
 * Task added as prototype to move AI pawn to safe point if it is needed.
 * Should be replaced in final solution.
 */
UCLASS()
class SERIOUSTANK_API UST_BTTask_GetNearestSafePoint : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	explicit UST_BTTask_GetNearestSafePoint(const FObjectInitializer& ObjectInitializer);
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
