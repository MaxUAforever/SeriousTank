#pragma once

#include "UObject/Object.h"
#include "BaseTaskStartCondition.generated.h"

UCLASS()
class QUESTSUBSYSTEM_API UBaseTaskStartCondition : public UObject
{
    GENERATED_BODY()

public:
    virtual bool CanBeStarted() { return true; };
};
