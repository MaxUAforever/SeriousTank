#include "Widgets/UserSettings/ST_AudioSettingsWidget.h"

#include "Engine/World.h"
#include "GameFramework/WorldSettings.h"
#include "Kismet/GameplayStatics.h"

void UST_AudioSettingsWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UST_AudioSettingsWidget::OnOverallVolumeChanged(float InValue)
{
	// TODO: Implement SoundMix System in C++ to implement this functions
	// World()->GetWorldSettings()->DefaultBaseSoundMix
}

void UST_AudioSettingsWidget::OnMusicVolumeChanged(float InValue)
{
}

void UST_AudioSettingsWidget::OnSoundsVolumeChanged(float InValue)
{
}

void UST_AudioSettingsWidget::OnUIVolumeChanged(float InValue)
{
}
