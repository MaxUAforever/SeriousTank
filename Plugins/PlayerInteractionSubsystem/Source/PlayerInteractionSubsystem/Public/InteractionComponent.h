#pragma once

#include "Components/SphereComponent.h"
#include "InteractionComponent.generated.h"

class UAnimMontage;
class UBaseInteractionAction;
class UInteractingComponent;
class UInteractionPointComponent;
class UInteractionUserWidget;
class UInteractionWidgetComponent;
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
	UPROPERTY(EditDefaultsOnly, Category = "Settings|Actions")
	TSubclassOf<UBaseInteractionAction> InteractionActionClass;

	UPROPERTY(EditAnywhere, Category = "Settings|Actions")
	UAnimMontage* ActionMontage;

	UPROPERTY(EditAnywhere, Category = "Settings|Actions")
	UAnimMontage* DeactivationMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Settings|Widget")
	bool bIsWidgetEnabled = true;

	UPROPERTY(EditDefaultsOnly, Category = "Settings|Widget", meta = (EditCondition = "bIsWidgetEnabled"))
	TSubclassOf<UInteractionUserWidget> InteractionWidgetClass;

private:
	UPROPERTY()
	UBaseInteractionAction* InteractionAction;
	UPlayerInteractionSubsystem* PlayerInteractionSubsystem;

	UPROPERTY()
	UInteractionWidgetComponent* InteractionWidgetComponent;

	TArray<UInteractionPointComponent*> InteractionPoints;
	
	bool bIsActive = true;

protected:
	virtual void BeginPlay() override;

public:
	const UBaseInteractionAction* GetAction() const { return InteractionAction; };
	bool ActivateAction(UInteractingComponent* InteractingComponent);
	bool DeactivateAction(UInteractingComponent* InteractingComponent);

	// Allows to control if component should trigger action on request. 
	void SetIsComponentActive(bool bInIsActive);

	const TArray<UInteractionPointComponent*>& GetInteractionPoints() const { return InteractionPoints; };
	
	const UInteractionPointComponent* GetClosestInteractionPoint(const FVector& Location) const;

protected:
	UFUNCTION()
	void HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void HandleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
