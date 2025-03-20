#pragma once

#include "Engine/DataTable.h"
#include "TableRowQuestInfo.generated.h"

class UBaseQuest;
class UQuestInfoDataAsset;

/**
 * 
 */
USTRUCT(BlueprintType)
struct QUESTSUBSYSTEM_API FTableRowQuestInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText QuestName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText QuestDescription;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UBaseQuest> BaseQuestClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UQuestInfoDataAsset> QuestInfoDataAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bIsRepeatable;
};
