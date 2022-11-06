#include "Widgets/Utils/ST_WidgetUtilsLibrary.h"

FText UST_WidgetUtilsLibrary::ConvertTimeToText(const float Time)
{
	int32 Minutes = FMath::TruncToInt(Time / 60);
	int32 Seconds = FMath::TruncToInt(Time) % 60;

	FString MinutesPrefix = Minutes < 10 ? "0" : "";
	FString MinutesStr = MinutesPrefix + FString::FromInt(Minutes);

	FString SecondsPrefix = Seconds < 10 ? "0" : "";
	FString SecondsStr = SecondsPrefix + FString::FromInt(Seconds);

	return FText::FromString(MinutesStr + ":" + SecondsStr);
}