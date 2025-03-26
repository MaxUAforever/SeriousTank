#pragma once

#include "Components/ActorComponent.h"
#include "QuestCoreTypes.h"
#include "QuestProvider.generated.h"


UCLASS()
class QUESTSUBSYSTEM_API UQuestProvider : public UActorComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	const TSet<int32>& GetQuestIDs() const { return QuestIDs; }
	const TSet<int32>& GetQuestTaskIDs() const { return QuestTasksIDs; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quests")
	TSet<int32> QuestIDs;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quests")
	TSet<int32> QuestTasksIDs;
};
