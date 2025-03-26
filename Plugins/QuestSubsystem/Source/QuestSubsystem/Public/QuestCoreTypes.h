#pragma once

DECLARE_LOG_CATEGORY_EXTERN(LogQuestSubsystem, Log, All);

using FQuestID = int32;
using FTaskID = int32;

UENUM(BlueprintType)
enum class EQuestCompleteRelust : uint8
{
    Succeeded,
    Failed,
    Cancelled
};
