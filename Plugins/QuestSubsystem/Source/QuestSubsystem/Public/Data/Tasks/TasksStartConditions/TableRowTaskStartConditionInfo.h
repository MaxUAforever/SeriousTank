#pragma once

#include "Engine/DataTable.h"
#include "QuestCoreTypes.h"
#include "TableRowTaskStartConditionInfo.generated.h"

class UBaseTaskStartCondition;

USTRUCT(BlueprintType)
struct QUESTSUBSYSTEM_API FTableRowTaskStartConditionInfo : public FTableRowBase
{
	GENERATED_BODY()

	FTableRowTaskStartConditionInfo() { static_assert(std::is_same_v<FTaskID, int32>, "ParentTaskID should have a type of FTaskID!"); };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 ParentTaskID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UBaseTaskStartCondition> BaseTaskStartConditionClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString ConditionParametersJSON;
};
