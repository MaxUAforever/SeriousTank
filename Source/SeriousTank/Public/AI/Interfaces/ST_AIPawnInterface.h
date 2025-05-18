#pragma once

#include "UObject/Object.h"

#include "UObject/Interface.h"
#include "ST_AIPawnInterface.generated.h"

UINTERFACE(MinimalAPI)
class UST_AIPawnInterface : public UInterface
{
    GENERATED_BODY()
};

class IST_AIPawnInterface
{
    GENERATED_BODY()

public:
    virtual bool IsAiming() const = 0;
    virtual bool IsAimingToFocusPoint() const = 0;
    virtual void AimToLocation(const FVector& Location) = 0;
};
