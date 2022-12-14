#include "Widgets/Gameplay/ST_WeaponReloadingWidget.h"

#include "Actors/Pawns/ST_BaseVehicle.h"
#include "Actors/Weapons/BaseWeapon.h"
#include "Actors/Weapons/MachineGunWeapon.h"
#include "Components/ST_WeaponSocketComponent.h"

#include "Components/Border.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Layout/Margin.h"
#include "TimerManager.h"

void UST_WeaponReloadingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetIsEnabled(false);
	SetVisibility(ESlateVisibility::Hidden);
    
    WeaponIndexBlock->SetText(FText::FromString(FString::FromInt(WeaponIndex + 1)));
    ClipAmmoCountBlock->SetVisibility(ESlateVisibility::Hidden);
    AmmoDelimiterBlock->SetVisibility(ESlateVisibility::Hidden);
    SelectionBorder->SetVisibility(WeaponIndex == 0 ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    
	AST_BaseVehicle* BaseVehiclePawn = GetOwningPlayerPawn<AST_BaseVehicle>();
	if (!BaseVehiclePawn)
	{
		return;
	}

    BaseVehiclePawn->OnWeaponSwitched.AddUObject(this, &ThisClass::OnWeaponSelected);
    
    TArray<UActorComponent*> WeaponSocketComponents;
	BaseVehiclePawn->GetComponents(UST_WeaponSocketComponent::StaticClass(), WeaponSocketComponents);
	if (WeaponIndex >= 0 && WeaponIndex < WeaponSocketComponents.Num())
	{
		UST_WeaponSocketComponent* WeaponSocket = Cast<UST_WeaponSocketComponent>(WeaponSocketComponents[WeaponIndex]);
		WeaponSocket->OnWeaponAdded.BindUObject(this, &ThisClass::OnWeaponAdded);
	}
}

void UST_WeaponReloadingWidget::OnWeaponAdded(ABaseWeapon* Weapon)
{
    if (!Weapon)
    {
        return;
    }
    
    SetIsEnabled(true);
	SetVisibility(ESlateVisibility::Visible);
    
	Weapon->OnReloadingStarted.BindUObject(this, &ThisClass::OnWeaponReloadingStarted, Weapon);
    Weapon->OnAmmoCountChanged.AddUObject(this, &ThisClass::UpdateTotalAmmoCount);
    
    UpdateTotalAmmoCount(Weapon->GetTotalAmmoCount());
    
    if (AMachineGunWeapon* MachineGunWeapon = Cast<AMachineGunWeapon>(Weapon))
    {
        ClipAmmoCountBlock->SetVisibility(ESlateVisibility::Visible);
        AmmoDelimiterBlock->SetVisibility(ESlateVisibility::Visible);
        UpdateClipAmmoCount(MachineGunWeapon->GetCurrentClipAmmoCount());
        
        MachineGunWeapon->OnClipAmmoCountChanged.AddUObject(this, &ThisClass::UpdateClipAmmoCount);
    }
}

void UST_WeaponReloadingWidget::OnWeaponReloadingStarted(ABaseWeapon* Weapon)
{
    FTimerDelegate ReloadingRefreshRateTimerDelegate = FTimerDelegate::CreateUObject(this, &ThisClass::UpdateReloadingProgressbar, Weapon);
	GetWorld()->GetTimerManager().SetTimer(WeaponReloadingRefreshRateHandler, ReloadingRefreshRateTimerDelegate, ReloadingRefreshRate, true);
    
    ReloadingProgressbar->SetVisibility(ESlateVisibility::Visible);
}

void UST_WeaponReloadingWidget::OnWeaponSelected(int32 Index)
{
    SelectionBorder->SetVisibility(WeaponIndex == Index ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UST_WeaponReloadingWidget::UpdateTotalAmmoCount(int32 TotalAmmoCount)
{
    TotalAmmoCountBlock->SetText(FText::FromString(FString::FromInt(TotalAmmoCount)));
}

void UST_WeaponReloadingWidget::UpdateClipAmmoCount(int32 ClipAmmoCount)
{
    ClipAmmoCountBlock->SetText(FText::FromString(FString::FromInt(ClipAmmoCount)));
}

void UST_WeaponReloadingWidget::UpdateReloadingProgressbar(ABaseWeapon* Weapon)
{
	if (!Weapon || !Weapon->IsReloading())
	{
        ReloadingProgressbar->SetPercent(0.f);
        ReloadingProgressbar->SetVisibility(ESlateVisibility::Hidden);
		GetWorld()->GetTimerManager().ClearTimer(WeaponReloadingRefreshRateHandler);
		return;
	}

	const float ReloadingTime = Weapon->GetReloadingRemainingTime();
    const float TotalReloadingTime = Weapon->GetTotalReloadingTime();
    
    ReloadingProgressbar->SetPercent(1.f - (ReloadingTime / TotalReloadingTime));
}
