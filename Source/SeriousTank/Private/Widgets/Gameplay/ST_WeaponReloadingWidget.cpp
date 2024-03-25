#include "Widgets/Gameplay/ST_WeaponReloadingWidget.h"

#include "Actors/Characters/Soldiers/ST_BaseSoldierCharacter.h"
#include "Actors/Pawns/ST_BaseVehicle.h"
#include "Actors/Weapons/ST_BaseWeapon.h"
#include "Actors/Weapons/ST_MachineGunWeapon.h"
#include "Components/ST_WeaponSocketComponent.h"
#include "Components/Weapons/ST_BaseWeaponsManagerComponent.h"

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
    
	APawn* OwnerPawn = GetOwningPlayerPawn();
	if (AST_BaseVehicle* BaseVehiclePawn = Cast<AST_BaseVehicle>(OwnerPawn))
	{
		BaseVehiclePawn->OnWeaponSwitched.AddUObject(this, &ThisClass::OnWeaponSelected);
	}
	else if (AST_BaseSoldierCharacter* BaseSoldierCharacter = Cast<AST_BaseSoldierCharacter>(OwnerPawn))
	{
		BaseSoldierCharacter->OnWeaponSwitched.AddUObject(this, &ThisClass::OnWeaponSelected);
	}
	else
	{
		return;
	}
    
    TArray<UActorComponent*> WeaponsManagerComponents;
	OwnerPawn->GetComponents(UST_BaseWeaponsManagerComponent::StaticClass(), WeaponsManagerComponents);
	if (WeaponsManagerComponents.Num() <= 0)
	{
		return;
	}

	UST_BaseWeaponsManagerComponent* WeaponsManagerComponent = Cast<UST_BaseWeaponsManagerComponent>(WeaponsManagerComponents[0]);
	if (WeaponsManagerComponent)
	{
		// TODO: Bind delegate to parent widget to preventing multiple broadcasting to each rloading widget
		WeaponsManagerComponent->OnWeaponAdded.AddUObject(this, &ThisClass::OnWeaponAdded);
	}
}

void UST_WeaponReloadingWidget::OnWeaponAdded(int32 InWeaponIndex, AST_BaseWeapon* Weapon)
{
    if (!Weapon || WeaponIndex != InWeaponIndex)
    {
        return;
    }
    
    SetIsEnabled(true);
	SetVisibility(ESlateVisibility::Visible);
    
	Weapon->OnReloadingStarted.AddUObject(this, &ThisClass::OnWeaponReloadingStarted, Weapon);
    Weapon->OnAmmoCountChanged.AddUObject(this, &ThisClass::UpdateTotalAmmoCount);
    
    UpdateTotalAmmoCount(Weapon->GetTotalAmmoCount());
    
	// TODO: Probably it is needed to get rid of such dependency - move whole method
	// to HUD with splitting for different weapon types.
    if (AST_MachineGunWeapon* MachineGunWeapon = Cast<AST_MachineGunWeapon>(Weapon))
    {
        ClipAmmoCountBlock->SetVisibility(ESlateVisibility::Visible);
        AmmoDelimiterBlock->SetVisibility(ESlateVisibility::Visible);
        UpdateClipAmmoCount(MachineGunWeapon->GetCurrentClipAmmoCount());
        
        MachineGunWeapon->OnClipAmmoCountChanged.AddUObject(this, &ThisClass::UpdateClipAmmoCount);
    }
}

void UST_WeaponReloadingWidget::OnWeaponReloadingStarted(AST_BaseWeapon* Weapon)
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

void UST_WeaponReloadingWidget::UpdateReloadingProgressbar(AST_BaseWeapon* Weapon)
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
