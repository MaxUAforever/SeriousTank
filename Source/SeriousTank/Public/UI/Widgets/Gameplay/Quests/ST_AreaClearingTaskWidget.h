#pragma once

#include "Blueprint/UserWidget.h"
#include "ST_AreaClearingTaskWidget.generated.h"

class UTextBlock;

UCLASS()
class SERIOUSTANK_API UST_AreaClearingTaskWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetWaveIndex(int32 InWaveIndex);
	void SetWaveEnemyAmount(int32 InWaveEnemyAmount);

protected:
	UPROPERTY(EditAnyWhere, meta = (BindWidget))
	UTextBlock* WaveIndexBlock;

	UPROPERTY(EditAnyWhere, meta = (BindWidget))
	UTextBlock* WaveEnemyAmountBlock;
};
