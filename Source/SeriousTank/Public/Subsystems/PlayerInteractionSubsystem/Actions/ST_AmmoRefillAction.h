#pragma once

#include "PlayerInteractionSubsystem/Public/Actions/BaseInterruptibleAction.h"
#include "ST_AmmoRefillAction.generated.h"

class UST_AmmoRefillSourceComponent;
class UST_BaseWeaponsManagerComponent;

UCLASS()
class SERIOUSTANK_API UST_AmmoRefillAction : public UBaseInterruptibleAction
{
	GENERATED_BODY()

public:
	virtual bool CanBeActivated(UInteractingComponent* InteractingComponent, UInteractionComponent* InteractionComponent) const override;
	
protected:
	virtual void OnActivationCompleted() override;
};
