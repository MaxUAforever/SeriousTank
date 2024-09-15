#include "GameFramework/AI/ST_AIController.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/ST_ViewAreaBoxComponent.h"
#include "Subsystems/HealthSubsystem/ST_HealthComponent.h"

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
}

void AST_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	SetupPerception(InPawn);
	SetupHealthSubsystem(InPawn);
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

void AST_AIController::OnTargetOverlapViewBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APawn* TargetPawn = Cast<APawn>(OtherActor))
	{
		if (TargetPawn->IsPlayerControlled())
		{
			OnTargetDetected(OtherActor);
		}
	}
}

void AST_AIController::OnTargetEndOverlapViewBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (APawn* TargetPawn = Cast<APawn>(OtherActor))
	{
		if (TargetPawn->IsPlayerControlled())
		{
			OnLostTarget(OtherActor);
		}
	}
}

void AST_AIController::OnTargetDetected(AActor* Target)
{
	if (GetBlackboardComponent())
	{
		GetBlackboardComponent()->SetValueAsObject(BBAttackTargetKey, Target);
	}
}

void AST_AIController::OnLostTarget(AActor* Target)
{
	if (GetBlackboardComponent())
	{
		GetBlackboardComponent()->SetValueAsObject(BBAttackTargetKey, nullptr);
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
