#pragma once

#include "AIController.h"
#include "ST_AIController.generated.h"

UCLASS()
class SERIOUSTANK_API AST_AIController : public AAIController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly)
	UBehaviorTree* DefaultBehaviourTree;
};
