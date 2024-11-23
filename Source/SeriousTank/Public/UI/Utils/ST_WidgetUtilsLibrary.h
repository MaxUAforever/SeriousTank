#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "ST_WidgetUtilsLibrary.generated.h"

UCLASS()
class SERIOUSTANK_API UST_WidgetUtilsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	static FText ConvertTimeToText(float Time);
};
