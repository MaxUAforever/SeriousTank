// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "ST_BTTask_GetOppositeLocation.generated.h"

UCLASS()
class SERIOUSTANK_API UST_BTTask_GetOppoiteLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	explicit UST_BTTask_GetOppoiteLocation(const FObjectInitializer& ObjectInitializer);
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Settings");
	float AcceptableRadius = 20.f;

	UPROPERTY(EditAnywhere, Category = "Settings");
	float Distance = 100.f;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	struct FBlackboardKeySelector BBOppositeLocationKey;
};
