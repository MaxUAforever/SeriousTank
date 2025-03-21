#include "UI/Widgets/Gameplay/Quests/ST_TargetShootingWidget.h"

#include "Components/TextBlock.h"

void UST_TargetShootingWidget::SetScore(int32 InScore)
{
	ScoreBlock->SetText(FText::FromString(FString::FromInt(InScore)));
}
