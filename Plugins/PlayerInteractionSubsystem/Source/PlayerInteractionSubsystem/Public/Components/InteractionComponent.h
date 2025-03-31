#pragma once

#include "Components/SphereComponent.h"
#include "InteractionComponent.generated.h"

class UAnimMontage;
class UBaseInteractionAction;
class UBaseInteractionActionDataAsset;
class UInteractingComponent;
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
	virtual void BeginPlay() override;

public:
	const TSubclassOf<UBaseInteractionAction> GetActionClass() const { return InteractionActionClass; };
	const UBaseInteractionActionDataAsset* GetInteractionActionDataAsset() const { return InteractionActionDataAsset; }

	// Allows to control if component should trigger action on request. 
	bool IsInteractionComponentActive() const { return bIsActive; }
	void SetIsInteractionComponentActive(bool bInIsActive);

	const TArray<FTransform>& GetRelativeInteractionPoints() const { return InteractionPoints; };
	TArray<FTransform> GetWorldInteractionPoints() const;

	TOptional<FTransform> GetClosestInteractionPoint(const FVector& Location) const;

	void UpdateWidgetData();

protected:
	UFUNCTION()
	void HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void HandleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Settings|Actions")
	TSubclassOf<UBaseInteractionAction> InteractionActionClass;

	UPROPERTY(EditDefaultsOnly, Category = "Settings|Actions")
	TObjectPtr<UBaseInteractionActionDataAsset> InteractionActionDataAsset;

	UPROPERTY(EditDefaultsOnly, Category = "Settings|Widget")
	bool bIsWidgetEnabled = true;

	UPROPERTY(EditDefaultsOnly, Category = "Settings|Widget", meta = (EditCondition = "bIsWidgetEnabled"))
	TSubclassOf<UInteractionUserWidget> InteractionWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (MakeEditWidget = "true"))
	TArray<FTransform> InteractionPoints;

private:
	TObjectPtr<UPlayerInteractionSubsystem> PlayerInteractionSubsystem;

	UPROPERTY()
	TObjectPtr<UInteractionWidgetComponent> InteractionWidgetComponent;

	bool bIsActive = true;
};
