#include "Widgets/UserSettings/ST_UserSettings.h"

#include "GameFramework/ST_BaseHUD.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"

void UST_UserSettings::NativeConstruct()
{
	Super::NativeConstruct();

	AudioTabButton->OnClicked.AddDynamic(this, &ThisClass::OnAudioTabButtonClicked);
	VideoTabButton->OnClicked.AddDynamic(this, &ThisClass::OnVideoTabButtonClicked);
	ApplyButton->OnClicked.AddDynamic(this, &ThisClass::OnApplyButtonClicked);
}

void UST_UserSettings::OnAudioTabButtonClicked()
{
	SettingWidgetsSwitcher->SetActiveWidget(AudioSettingsWidget);
}

void UST_UserSettings::OnVideoTabButtonClicked()
{
	SettingWidgetsSwitcher->SetActiveWidget(VideoSettingsWidget);
}

void UST_UserSettings::OnApplyButtonClicked()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(World, 0)))
	{
		if (AST_BaseHUD* BaseHUD = PlayerController->GetHUD<AST_BaseHUD>())
		{
			BaseHUD->SwitchFromSettingsWidget();
		}
	}
}
