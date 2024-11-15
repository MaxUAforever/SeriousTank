#include "GameFramework/AI/ST_AIController.h"

#include "Actors/Weapons/ST_BaseWeapon.h"
#include "AIPatrollingSubsystem/Public/Components/AIPatrollingComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/ST_ViewAreaBoxComponent.h"
#include "Components/Weapons/ST_BaseWeaponsManagerComponent.h"
#include "GameFramework/Gameplay/ST_GameplayGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystems/HealthSubsystem/ST_HealthComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISenseConfig.h"
#include "Perception/AISenseConfig_Sight.h"

AST_AIController::AST_AIController()
{
	PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>("PerceptionComponent");
	PatrollingComponent = CreateDefaultSubobject<UAIPatrollingComponent>("PatrollingComponent");
}

void AST_AIController::BeginPlay()
{
	Super::BeginPlay();

	if (GetBlackboardComponent())
	{
		GetBlackboardComponent()->SetValueAsEnum(BBSightPerceptionTypeKey, static_cast<uint8>(ViewPerceptionType));
	}

	if (DefaultBehaviourTree)
	{
		RunBehaviorTree(DefaultBehaviourTree);
	}

	SetPerceptionComponent(*PerceptionComp);
	GetPerceptionComponent()->Deactivate();

	if (PatrollingComponent)
	{
		OnPatrollingStateChaned(PatrollingComponent->IsPatrollingActive());
		PatrollingComponent->OnIsActiveChanged.AddUObject(this, &ThisClass::OnPatrollingStateChaned);
	}
}

void AST_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (GetWorld()->HasBegunPlay())
	{
		SetupPawnSettings();
	}
	else if (AST_GameplayGameState* GameplayGameState = Cast<AST_GameplayGameState>(UGameplayStatics::GetGameState(this)))
	{
		GameplayGameState->OnPreStartCountdownEndedDelegate.AddUObject(this, &ThisClass::SetupPawnSettings);
	}
}

void AST_AIController::SetupPawnSettings()
{
	SetupPerception(GetPawn());
	SetupHealthSubsystem(GetPawn());
	SetupWeaponsComponent(GetPawn());
}

void AST_AIController::SetupPerception(APawn* InPawn)
{
	if (!IsValid(InPawn))
	{
		return;
	}

	if (ViewPerceptionType == EViewPerceptionType::PlayerView)
	{
		if (UST_ViewAreaBoxComponent* ViewAreaBoxComponent = Cast<UST_ViewAreaBoxComponent>(InPawn->GetComponentByClass(UST_ViewAreaBoxComponent::StaticClass())))
		{
			ViewAreaBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnTargetOverlapViewBox);
			ViewAreaBoxComponent->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnTargetEndOverlapViewBox);
		}
	}
	else if (ViewPerceptionType == EViewPerceptionType::AIPerception)
	{
		if (!GetPerceptionComponent())
		{
			return;
		}
			
		UAISenseConfig_Sight* SightConfig = Cast<UAISenseConfig_Sight>(GetPerceptionComponent()->GetSenseConfig(UAISense::GetSenseID<UAISense_Sight>()));
		if (!SightConfig)
		{
			return;
		}

		if (UST_ViewAreaBoxComponent* ViewAreaBoxComponent = Cast<UST_ViewAreaBoxComponent>(InPawn->GetComponentByClass(UST_ViewAreaBoxComponent::StaticClass())))
		{
			ChangeSightRadius(ViewAreaBoxComponent->GetFrontViewDistance());
			ViewAreaBoxComponent->OnFrontViewDistanceChanged.AddUObject(this, &ThisClass::ChangeSightRadius);
		}

		GetPerceptionComponent()->Activate();
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ThisClass::OnSensePerceptionTriggered);
	}
}

void AST_AIController::SetupHealthSubsystem(APawn* InPawn)
{
	if (!IsValid(InPawn))
	{
		return;
	}

	if (UST_HealthComponent* HealthComponent = InPawn->GetComponentByClass<UST_HealthComponent>())
	{
		HealthComponent->OnHealthValueChangedDelegate.AddUObject(this, &ThisClass::OnHealthChanged);
	}
}

void AST_AIController::SetupWeaponsComponent(APawn* InPawn)
{
	if (!IsValid(InPawn))
	{
		return;
	}

	UST_BaseWeaponsManagerComponent* WeaponsManagerComponent = InPawn->GetComponentByClass<UST_BaseWeaponsManagerComponent>();
	if (!WeaponsManagerComponent)
	{
		return;
	}
		
	WeaponsManagerComponent->OnWeaponAdded.AddUObject(this, &ThisClass::OnWeaponAdded);
	for (AST_BaseWeapon* Weapon : WeaponsManagerComponent->GetWeapons())
	{
		if (Weapon->GetTotalAmmoCount() > 0)
		{
			GetBlackboardComponent()->SetValueAsBool(BBCanAttackKey, true);
			return;
		}
	}
}

void AST_AIController::OnTargetOverlapViewBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OnTargetDetected(OtherActor);
}

void AST_AIController::OnTargetEndOverlapViewBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	OnTargetLost(OtherActor);
}

void AST_AIController::OnSensePerceptionTriggered(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{
		OnTargetDetected(Actor);
	}
	else
	{
		OnTargetLost(Actor);
	}
}

void AST_AIController::ChangeSightRadius(float InSightRadius)
{
	if (GetPerceptionComponent())
	{
		if (UAISenseConfig_Sight* SightConfig = Cast<UAISenseConfig_Sight>(GetPerceptionComponent()->GetSenseConfig(UAISense::GetSenseID<UAISense_Sight>())))
		{
			SightConfig->SightRadius = InSightRadius * PerceptionSightRadiusScale;
			SightConfig->LoseSightRadius = SightConfig->SightRadius * 1.2f;
			GetPerceptionComponent()->ConfigureSense(*SightConfig);
		}
	}
}

void AST_AIController::OnPatrollingStateChaned(bool bIsActive)
{
	if (GetBlackboardComponent())
	{
		GetBlackboardComponent()->SetValueAsBool(BBCanPatrol, bIsActive);
	}
}

void AST_AIController::OnWeaponAdded(int32 WeaponIndex, AST_BaseWeapon* Weapon)
{
	if (!Weapon || !GetBlackboardComponent())
	{
		return;
	}

	if (Weapon->GetTotalAmmoCount() > 0)
	{
		GetBlackboardComponent()->SetValueAsBool(BBCanAttackKey, true);
	}
}

void AST_AIController::OnTargetDetected(AActor* Target)
{
	if (!GetBlackboardComponent())
	{
		return;
	}

	if (APawn* TargetPawn = Cast<APawn>(Target))
	{
		if (TargetPawn->IsPlayerControlled())
		{
			GetBlackboardComponent()->SetValueAsObject(BBAttackTargetKey, Target);
		}
	}
}

void AST_AIController::OnTargetLost(AActor* Target)
{
	if (!GetBlackboardComponent())
	{
		return;
	}

	if (APawn* TargetPawn = Cast<APawn>(Target))
	{
		if (TargetPawn->IsPlayerControlled())
		{
			GetBlackboardComponent()->SetValueAsObject(BBAttackTargetKey, nullptr);
		}
	}
}

void AST_AIController::OnHealthChanged(float CurrentHealthValue, EHealthChangingType HealthChangingType)
{
	if (FMath::IsNearlyZero(CurrentHealthValue))
	{
		UBehaviorTreeComponent* BTComponent = GetComponentByClass<UBehaviorTreeComponent>();
		if (!BTComponent)
		{
			return;
		}

		BTComponent->StopTree();
		ClearFocus(EAIFocusPriority::Gameplay);
	}
}
