#pragma once

#include "Subsystems/WorldSubsystem.h"
#include "Subsystems/AIManagerSubsystem/ST_AIManagerTypes.h"
#include "ST_AIInteractionsManagerSubsystem.generated.h"


struct FInteractionTypeInfo
{
	TSet<TObjectPtr<const AActor>> InteractionActors;
};

class UBaseInteractionAction;
class UInteractionComponent;

UCLASS()
class SERIOUSTANK_API UST_AIInteractionsManagerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	const TSet<TObjectPtr<const AActor>>* GetInteractionActors(EAIInteractionType InteractionType) const;

private:
	void InitInteractionItems(const UWorld& InWorld);
	void AddInteractionComponent(const UInteractionComponent* InteractionComponent);
	void RemoveInteractionComponent(const UInteractionComponent* InteractionComponent);

	void OnIsInteractionComponentActiveChanged(const UInteractionComponent* InteractionComponent, bool bIsActive);
	static EAIInteractionType GetInteractionTypeByClass(TSubclassOf<UBaseInteractionAction> ActionClass);

private:
	TMap<EAIInteractionType, FInteractionTypeInfo> InteractionsInfo;
};
