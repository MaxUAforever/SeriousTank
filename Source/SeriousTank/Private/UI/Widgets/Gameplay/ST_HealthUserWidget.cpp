#include "UI/Widgets/Gameplay/ST_HealthUserWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "GameFramework/Pawn.h"
#include "Subsystems/HealthSubsystem/Components/ST_HealthComponent.h"

void UST_HealthUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UpdateView();
}

void UST_HealthUserWidget::SetMaxHealthValue(float InMaxHealth)
{
	if (InMaxHealth <= 0.f)
	{
		return;
	}

	MaxHealthValue = InMaxHealth;

	UpdateView();
}

void UST_HealthUserWidget::SetHealthValue(float InHealth)
{
	if (InHealth <= 0.f)
	{
		return;
	}

	CurrentHealthValue = InHealth;

	UpdateView();
}

void UST_HealthUserWidget::SetProgressBarColor(const FLinearColor& InColor)
{
	if (HealthProgressBar)
	{
		HealthProgressBar->SetFillColorAndOpacity(InColor);
	}
}

void UST_HealthUserWidget::UpdateView()
{
	HealthProgressBar->SetPercent(CurrentHealthValue / MaxHealthValue);
	HealthValueBlock->SetText(FText::FromString(FString::Printf(TEXT("%0.1f / %0.1f"), CurrentHealthValue, MaxHealthValue)));
}
