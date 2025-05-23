#include "UI/Widgets/Gameplay/ST_WeaponReloadingWidget.h"

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
    
	APlayerController* PlayerController = GetOwningPlayer();
	if (!PlayerController)
	{
		return;
	}

	PlayerController->OnPossessedPawnChanged.AddDynamic(this, &ThisClass::OnPawnChanged);
	OnPawnChanged(nullptr, PlayerController->GetPawn());
}

void UST_WeaponReloadingWidget::OnWeaponAdded(int32 InWeaponIndex, AST_BaseWeapon* Weapon)
{
    if (!Weapon || WeaponIndex != InWeaponIndex)
    {
        return;
    }
    
	APlayerController* PlayerController = GetOwningPlayer();
	if (!PlayerController)
	{
		return;
	}

    SetIsEnabled(true);
	SetVisibility(ESlateVisibility::Visible);
    
	OnReloadingStartedDelegateHandle = WeaponsManagerComponent->OnWeaponReloadingStartedDelegate.AddUObject(this, &ThisClass::OnWeaponReloadingStarted);
	OnAmmoCountChangedDelegateHandle = Weapon->OnAmmoCountChangedDelegate.AddUObject(this, &ThisClass::UpdateTotalAmmoCount);
    
	UpdateTotalAmmoCount(0, Weapon->GetTotalAmmoCount());
    
	if (Weapon->IsReloading())
	{
		OnWeaponReloadingStarted(WeaponIndex, Weapon);
	}

	if (WeaponsManagerComponent)
	{
		SelectionBorder->SetVisibility(WeaponIndex == WeaponsManagerComponent->GetCurrentWeaponIndex() ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}

	// TODO: Probably it is needed to get rid of such dependency - move whole method
	// to HUD with splitting for different weapon types.
    if (AST_MachineGunWeapon* MachineGunWeapon = Cast<AST_MachineGunWeapon>(Weapon))
    {
        ClipAmmoCountBlock->SetVisibility(ESlateVisibility::Visible);
        AmmoDelimiterBlock->SetVisibility(ESlateVisibility::Visible);
        UpdateClipAmmoCount(MachineGunWeapon->GetCurrentClipAmmoCount());
        
		OnClipAmmoCountChangedDelegateHandle = MachineGunWeapon->OnClipAmmoCountChanged.AddUObject(this, &ThisClass::UpdateClipAmmoCount);
    }
}

void UST_WeaponReloadingWidget::OnWeaponRemoved(int32 InWeaponIndex, AST_BaseWeapon* Weapon)
{
	if (!Weapon || WeaponIndex != InWeaponIndex)
	{
		return;
	}

	SetIsEnabled(false);
	SetVisibility(ESlateVisibility::Hidden);

	ClipAmmoCountBlock->SetVisibility(ESlateVisibility::Hidden);
	AmmoDelimiterBlock->SetVisibility(ESlateVisibility::Hidden);

	Weapon->OnReloadingStartedDelegate.Remove(OnReloadingStartedDelegateHandle);
	Weapon->OnAmmoCountChangedDelegate.Remove(OnAmmoCountChangedDelegateHandle);

	GetWorld()->GetTimerManager().ClearTimer(WeaponReloadingRefreshRateHandler);
	ReloadingProgressbar->SetPercent(0.f);
	ReloadingProgressbar->SetVisibility(ESlateVisibility::Hidden);

	if (AST_MachineGunWeapon* MachineGunWeapon = Cast<AST_MachineGunWeapon>(Weapon))
	{
		MachineGunWeapon->OnClipAmmoCountChanged.Remove(OnClipAmmoCountChangedDelegateHandle);
	}
}

void UST_WeaponReloadingWidget::OnWeaponReloadingStarted(int32 InWeaponIndex, AST_BaseWeapon* Weapon)
{
	if (WeaponIndex != InWeaponIndex)
	{
		return;
	}

	// TODO: Probably it's better to use tick function with customized interval
    FTimerDelegate ReloadingRefreshRateTimerDelegate = FTimerDelegate::CreateUObject(this, &ThisClass::UpdateReloadingProgressbar);
	GetWorld()->GetTimerManager().SetTimer(WeaponReloadingRefreshRateHandler, ReloadingRefreshRateTimerDelegate, ReloadingRefreshRate, true);
    
    ReloadingProgressbar->SetVisibility(ESlateVisibility::Visible);
}

void UST_WeaponReloadingWidget::OnWeaponSelected(int32 PreviousWeaponIndex, int32 NewWeaponIndex)
{
    SelectionBorder->SetVisibility(WeaponIndex == NewWeaponIndex ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UST_WeaponReloadingWidget::UpdateTotalAmmoCount(int32 OldAmmoCount, int32 TotalAmmoCount)
{
    TotalAmmoCountBlock->SetText(FText::FromString(FString::FromInt(TotalAmmoCount)));
}

void UST_WeaponReloadingWidget::UpdateClipAmmoCount(int32 ClipAmmoCount)
{
    ClipAmmoCountBlock->SetText(FText::FromString(FString::FromInt(ClipAmmoCount)));
}

void UST_WeaponReloadingWidget::OnPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	if (!NewPawn)
	{
		return;
	}

	if (OldPawn)
	{
		UST_BaseWeaponsManagerComponent* OldWeaponsManagerComponent = OldPawn->GetComponentByClass<UST_BaseWeaponsManagerComponent>();
		if (OldWeaponsManagerComponent)
		{
			OldWeaponsManagerComponent->OnWeaponAddedDelegate.Remove(OnWeaponAddedDelegateHandle);
			OldWeaponsManagerComponent->OnPreWeaponRemovedDelegate.Remove(OnWeaponRemovedDelegateHandle);
			OldWeaponsManagerComponent->OnWeaponSwitchingStartedDelegate.Remove(OnWeaponSwitchedDelegateHandle);
		
			if (AST_BaseWeapon* OldWeapon = OldWeaponsManagerComponent->GetWeapon(WeaponIndex))
			{
				OnWeaponRemoved(WeaponIndex, OldWeapon);
			}
		}
	}

	WeaponsManagerComponent = NewPawn->GetComponentByClass<UST_BaseWeaponsManagerComponent>();
	if (WeaponsManagerComponent)
	{
		// TODO: Bind delegate to parent widget to preventing multiple broadcasting to each rloading widget
		OnWeaponAddedDelegateHandle = WeaponsManagerComponent->OnWeaponAddedDelegate.AddUObject(this, &ThisClass::OnWeaponAdded);
		OnWeaponRemovedDelegateHandle = WeaponsManagerComponent->OnPreWeaponRemovedDelegate.AddUObject(this, &ThisClass::OnWeaponRemoved);
		OnWeaponSwitchedDelegateHandle = WeaponsManagerComponent->OnWeaponSwitchingStartedDelegate.AddUObject(this, &ThisClass::OnWeaponSelected);
	
		if (AST_BaseWeapon* NewWeapon = WeaponsManagerComponent->GetWeapon(WeaponIndex))
		{
			OnWeaponAdded(WeaponIndex, NewWeapon);
		}
	}
}

void UST_WeaponReloadingWidget::UpdateReloadingProgressbar()
{
	if (!IsValid(WeaponsManagerComponent))
	{
		return;
	}

	AST_BaseWeapon* Weapon = WeaponsManagerComponent->GetWeapon(WeaponIndex);
	if (!IsValid(Weapon) || !Weapon->IsReloading())
	{
		ReloadingProgressbar->SetPercent(0.f);
		ReloadingProgressbar->SetVisibility(ESlateVisibility::Hidden);
		GetWorld()->GetTimerManager().ClearTimer(WeaponReloadingRefreshRateHandler);
		return;
	}

	const float ReloadingTime = WeaponsManagerComponent->GetWeaponReloadingTime(WeaponIndex);
	const float TotalReloadingTime = WeaponsManagerComponent->GetWeaponTotalReloadingTime(WeaponIndex);

	ReloadingProgressbar->SetPercent(1.f - (ReloadingTime / TotalReloadingTime));
}
