// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "ST_BTTask_ExitVehicle.generated.h"

UCLASS()
class SERIOUSTANK_API UST_BTTask_ExitVehicle : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	explicit UST_BTTask_ExitVehicle(const FObjectInitializer& ObjectInitializer);
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
