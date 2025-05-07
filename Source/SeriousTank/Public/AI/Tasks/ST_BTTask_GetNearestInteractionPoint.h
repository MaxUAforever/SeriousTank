// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Subsystems/AIManagerSubsystem/ST_AIManagerTypes.h"
#include "ST_BTTask_GetNearestInteractionPoint.generated.h"

UCLASS()
class SERIOUSTANK_API UST_BTTask_GetNearestInteractionPoint : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	explicit UST_BTTask_GetNearestInteractionPoint(const FObjectInitializer& ObjectInitializer);
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	const AActor* FindNearestInteractionActor(const APawn* OwnerPawn, EAIInteractionType InteractionType) const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings");
	TArray<EAIInteractionType> InteractionTypes;
};
