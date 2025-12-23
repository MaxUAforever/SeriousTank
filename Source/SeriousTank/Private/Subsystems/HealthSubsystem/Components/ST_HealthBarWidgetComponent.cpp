#include "Subsystems/HealthSubsystem/Components/ST_HealthBarWidgetComponent.h"

#include "GameFramework/AI/ST_AIController.h"
#include "GameFramework/Pawn.h"
#include "GenericTeamAgentInterface.h"
#include "Subsystems/HealthSubsystem/Components/ST_HealthComponent.h"
#include "UI/Widgets/Gameplay/ST_HealthUserWidget.h"

void UST_HealthBarWidgetComponent::BeginPlay()
{
	Super::BeginPlay();

	UST_HealthComponent* HealthComponent = GetOwner()->GetComponentByClass<UST_HealthComponent>();
	if (!IsValid(HealthComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("UST_HealthBarWidgetComponent::BeginPlay: HealthComponent is not valid"));
		return;
	}

	SetWidgetClass(HealthBarWidgetClass);
	SetWidgetSpace(EWidgetSpace::Screen);
	SetDrawAtDesiredSize(true);
	SetRelativeLocation(FVector{ 0.f, 0.f, HeightOffset });

	HealthBarWidget = Cast<UST_HealthUserWidget>(GetUserWidgetObject());
	if (!HealthBarWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("UST_HealthBarWidgetComponent::OnHealthValueChanged: failed to get Widget"));
		return;
	}

	HealthComponent->OnHealthValueChangedDelegate.AddUObject(this, &ThisClass::OnHealthValueChanged);
}

void UST_HealthBarWidgetComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UST_HealthComponent* HealthComponent = GetOwner()->GetComponentByClass<UST_HealthComponent>();
	if (IsValid(HealthComponent))
	{
		HealthComponent->OnHealthValueChangedDelegate.RemoveAll(this);
		SetVisibility(false);
	}
}

void UST_HealthBarWidgetComponent::SetupColor(ETeamRelationType TeamRelationType)
{
	if (!IsValid(HealthBarWidget))
	{
		UE_LOG(LogTemp, Warning, TEXT("UST_HealthBarWidgetComponent::SetupColor: HealthBarWidget is not valid"));
		return;
	}

	FLinearColor CurrentHealthBarColor;
	switch (TeamRelationType)
	{
	case ETeamRelationType::Ally:
		CurrentHealthBarColor = AllyHealthBarColor;
		break;
	case ETeamRelationType::Enemy:
		CurrentHealthBarColor = EnemyHealthBarColor;
		break;
	case ETeamRelationType::Neutral:
		CurrentHealthBarColor = NeutralHealthBarColor;
		break;
	}
		
	HealthBarWidget->SetProgressBarColor(CurrentHealthBarColor);
}

void UST_HealthBarWidgetComponent::OnHealthValueChanged(float NewHealthValue, EHealthChangingType HealthChangingType)
{
	if (IsValid(HealthBarWidget))
	{
		if (FMath::IsNearlyZero(NewHealthValue))
		{
			SetVisibility(false);
		}
		else
		{
			HealthBarWidget->SetHealthValue(NewHealthValue);
		}
	}
}
