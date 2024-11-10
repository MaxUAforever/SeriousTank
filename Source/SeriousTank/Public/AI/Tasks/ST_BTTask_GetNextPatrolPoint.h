// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "ST_BTTask_GetNextPatrolPoint.generated.h"

enum class EDirectionType : uint8;

struct FBTGetNextPatrolPointTaskMemory
{
	int32 CurrentPatrolPoint;
	EDirectionType CurrentDirectionType;
};

/**
 * 
 */
UCLASS()
class SERIOUSTANK_API UST_BTTask_GetNextPatrolPoint : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	explicit UST_BTTask_GetNextPatrolPoint(const FObjectInitializer& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual uint16 GetInstanceMemorySize() const override;
};
