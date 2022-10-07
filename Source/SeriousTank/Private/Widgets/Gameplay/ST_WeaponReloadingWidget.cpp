#include "Widgets/Gameplay/ST_WeaponReloadingWidget.h"

#include "Actors/Pawns/ST_BaseVehicle.h"
#include "Actors/Weapons/BaseWeapon.h"
#include "Components/ST_WeaponSocketComponent.h"

#include "Components/TextBlock.h"
#include "TimerManager.h"

void UST_WeaponReloadingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetIsEnabled(false);
	SetVisibility(ESlateVisibility::Hidden);

	AST_BaseVehicle* BaseVehiclePawn = GetOwningPlayerPawn<AST_BaseVehicle>();
	if (!BaseVehiclePawn)
	{
		return;
	}

	TArray<UActorComponent*> WeaponSocketActors;
	BaseVehiclePawn->GetComponents(UST_WeaponSocketComponent::StaticClass(), WeaponSocketActors);
	
	if (WeaponIndex >= 0 && WeaponIndex < WeaponSocketActors.Num())
	{
		UST_WeaponSocketComponent* WeaponSocket = Cast<UST_WeaponSocketComponent>(WeaponSocketActors[WeaponIndex]);
		WeaponSocket->OnWeaponAdded.BindUObject(this, &ThisClass::OnWeaponAdded);
	}
}

void UST_WeaponReloadingWidget::OnWeaponAdded(ABaseWeapon* Weapon)
{
	SetIsEnabled(true);
	SetVisibility(ESlateVisibility::HitTestInvisible);

	Weapon->OnReloadingStarted.BindUObject(this, &ThisClass::OnWeaponReloadingStarted, Weapon);
}

void UST_WeaponReloadingWidget::OnWeaponReloadingStarted(ABaseWeapon* Weapon)
{
	FTimerDelegate ReloadingRefreshRateTimerDelegate = FTimerDelegate::CreateUObject(this, &ThisClass::UpdateReloadingTextBlock, Weapon);
	GetWorld()->GetTimerManager().SetTimer(WeaponReloadingRefreshRateHandler, ReloadingRefreshRateTimerDelegate, ReloadingRefreshRate, true);
}

void UST_WeaponReloadingWidget::UpdateReloadingTextBlock(ABaseWeapon* Weapon)
{
	if (!Weapon || !Weapon->IsReloading())
	{
		GetWorld()->GetTimerManager().ClearTimer(WeaponReloadingRefreshRateHandler);
		return;
	}

	const float ReloadingTime = Weapon->GetReloadingRemainingTime();
	WeaponReloadingBlock->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), ReloadingTime)));
}
