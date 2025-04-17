#pragma once

#include "PlayerInteractionSubsystem/Public/Actions/BaseAutomaticAction.h"
#include "ST_WeaponPickUpAction.generated.h"

class AST_BaseWeapon;
class UST_BaseWeaponsManagerComponent;

UCLASS()
class SERIOUSTANK_API UST_WeaponPickUpAction : public UBaseAutomaticAction
{
	GENERATED_BODY()

public:
	virtual bool CanBeActivatedAutomatically(UInteractingComponent* InteractingComponent, UInteractionComponent* InteractionComponent) const override;
	virtual bool CanBeActivated(UInteractingComponent* InteractingComponent, UInteractionComponent* InteractionComponent) const override;

	virtual bool ActivateAutomatically(UInteractingComponent* InteractingComponent, UInteractionComponent* InteractionComponent) override;

	virtual FText GetActionDescription() const override;

private:
	AST_BaseWeapon* GetWeaponByClass(UST_BaseWeaponsManagerComponent* WeaponsManagerComponent, TSubclassOf<AST_BaseWeapon> WeaponClass) const;
};
