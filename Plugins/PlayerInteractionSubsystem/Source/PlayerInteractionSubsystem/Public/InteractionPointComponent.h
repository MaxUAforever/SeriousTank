#pragma once

#include "Components/SceneComponent.h"
#include "InteractionPointComponent.generated.h"

/**
 * Component that can be added to interaction component to specify the point where the actor.
 * should be placed during the interaction.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLAYERINTERACTIONSUBSYSTEM_API UInteractionPointComponent : public USceneComponent
{
	GENERATED_BODY()

protected:
	/* Distance to the interactable place of object. */
	UPROPERTY(EditAnywhere)
	float InteractionDistance;

public:
	float GetInteractionDistance() const { return InteractionDistance; }	
};
