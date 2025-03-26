#include "UI/Widgets/Gameplay/Quests/ST_AreaClearingTaskWidget.h"

#include "Components/TextBlock.h"

void UST_AreaClearingTaskWidget::SetWaveIndex(int32 InWaveIndex)
{
	WaveIndexBlock->SetText(FText::FromString(FString::FromInt(InWaveIndex)));
}

void UST_AreaClearingTaskWidget::SetWaveEnemyAmount(int32 InWaveEnemyAmount)
{
	WaveEnemyAmountBlock->SetText(FText::FromString(FString::FromInt(InWaveEnemyAmount)));
}
