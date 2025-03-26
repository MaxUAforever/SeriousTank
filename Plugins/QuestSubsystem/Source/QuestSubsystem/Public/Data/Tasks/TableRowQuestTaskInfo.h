#pragma once

#include "Engine/DataTable.h"
#include "QuestCoreTypes.h"
#include "TableRowQuestTaskInfo.generated.h"

class UBaseTaskStartCondition;
class UBaseQuestTask;
class UQuestTaskInfoDataAsset;

USTRUCT(BlueprintType)
struct QUESTSUBSYSTEM_API FTableRowQuestTaskInfo : public FTableRowBase
{
	GENERATED_BODY()

	FTableRowQuestTaskInfo() { static_assert(std::is_same_v<FQuestID, int32>, "ParentQuestID should have a type of FTaskID!"); };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 ParentQuestID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText TaskName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText TaskDescription;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UBaseQuestTask> BaseQuestTaskClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UQuestTaskInfoDataAsset> QuestTaskInfoDataAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bShouldBeTracked = true;
};
