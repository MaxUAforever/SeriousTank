#pragma once

#include "Components/SphereComponent.h"
#include "InteractionComponent.generated.h"

class UBaseInteractionAction;
class UInteractingComponent;
class UPlayerInteractionSubsystem;

/**
 * Component that should be added to actors that should provide interaction action.
 * Overlapping of sphere component by actor with IneractingComponent will be registered in InteractionSubsystem
 * And allow it to initiate interactionProcess
 */
UCLASS()
class PLAYERINTERACTIONSUBSYSTEM_API UInteractionComponent : public USphereComponent
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UBaseInteractionAction> InteractionActionClass;

private:
	UPROPERTY()
	UBaseInteractionAction* InteractionAction;

	UPlayerInteractionSubsystem* PlayerInteractionSubsystem;

protected:
	virtual void BeginPlay() override;

public:
	void ActivateAction(UInteractingComponent* InteractingComponent);

protected:
	UFUNCTION()
	void HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void HandleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
