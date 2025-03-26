#include "UI/ViewModel/Gameplay/ST_VMHealth.h"

#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystems/HealthSubsystem/ST_HealthComponent.h"

void UST_VMHealth::Initialize()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetOuter(), 0);
	if (!IsValid(PlayerController))
	{
		return;
	}

	PlayerController->OnPossessedPawnChanged.AddDynamic(this, &ThisClass::OnPawnChanged);
	APawn* Pawn = PlayerController->GetPawn();
	if (IsValid(Pawn))
	{
		OnPawnChanged(nullptr, Pawn);
	}
}

void UST_VMHealth::OnHealthChanged(float CurrentHealthValue, EHealthChangingType HealthChangingType) const
{
	OnHealthChangedDelegate.ExecuteIfBound(CurrentHealthValue);
}

void UST_VMHealth::OnMaxHealthChanged(float MaxHealthValue) const
{
	OnMaxHealthChangedDelegate.ExecuteIfBound(MaxHealthValue);
}

void UST_VMHealth::OnPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	if (!IsValid(NewPawn))
	{
		return;
	}

	if (IsValid(OldPawn))
	{
		if (UST_HealthComponent* HealthComponent = OldPawn->GetComponentByClass<UST_HealthComponent>())
		{
			HealthComponent->OnHealthValueChangedDelegate.Remove(OnHealthChangedDelegateHandle);
			HealthComponent->OnMaxHealthValueChangedDelegate.Remove(OnMaxHealthChangedDelegateHandle);
		}
	}

	if (UST_HealthComponent* NewHealthComponent = NewPawn->GetComponentByClass<UST_HealthComponent>())
	{
		OnHealthChangedDelegateHandle = NewHealthComponent->OnHealthValueChangedDelegate.AddUObject(this, &ThisClass::OnHealthChanged);
		OnMaxHealthChangedDelegateHandle = NewHealthComponent->OnMaxHealthValueChangedDelegate.AddUObject(this, &ThisClass::OnMaxHealthChanged);
	
		OnHealthChanged(NewHealthComponent->GetCurrentHealth(), EHealthChangingType::Healing);
		OnMaxHealthChanged(NewHealthComponent->GetMaxHealth());
	}
}
