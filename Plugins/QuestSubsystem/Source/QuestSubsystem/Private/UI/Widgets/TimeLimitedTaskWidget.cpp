#include "UI/Widgets/TimeLimitedTaskWidget.h"

#include "Components/TextBlock.h"

namespace
{

FText ConvertTimeToText(const float Time)
{
	int32 Minutes = FMath::TruncToInt(Time / 60);
	int32 Seconds = FMath::TruncToInt(Time) % 60;

	FString MinutesPrefix = Minutes < 10 ? "0" : "";
	FString MinutesStr = MinutesPrefix + FString::FromInt(Minutes);

	FString SecondsPrefix = Seconds < 10 ? "0" : "";
	FString SecondsStr = SecondsPrefix + FString::FromInt(Seconds);

	return FText::FromString(MinutesStr + ":" + SecondsStr);
}

} // anonymous namespace

void UTimeLimitedTaskWidget::SetRemainingTime(const float InRemainingTime)
{
	RemainingTimeBlock->SetText(ConvertTimeToText(InRemainingTime));
}
