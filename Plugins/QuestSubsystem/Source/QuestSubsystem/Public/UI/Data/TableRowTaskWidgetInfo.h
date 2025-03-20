#pragma once

#include "Engine/DataTable.h"
#include "Templates/SubclassOf.h"
#include "TableRowTaskWidgetInfo.generated.h"

class UUserWidget;
class UViewModelBase;

USTRUCT(BlueprintType)
struct QUESTSUBSYSTEM_API FTableRowTaskWidgetInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUserWidget> QuestTaskWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UViewModelBase> QuestTaskViewModelClass;
};
