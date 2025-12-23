#include "Actors/AIPatrolPath.h"

#include "AIPatrollingSubsystem.h"
#include "Components/BillboardComponent.h"

AAIPatrolPath::AAIPatrolPath()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SetRootComponent(SceneComponent);

	EditorBillboard = CreateDefaultSubobject<UBillboardComponent>("Billboard");
	EditorBillboard->SetupAttachment(SceneComponent);
}

void AAIPatrolPath::BeginPlay()
{
	Super::BeginPlay();

	UAIPatrollingSubsystem* PatrolingSubsystem = GetWorld()->GetSubsystem<UAIPatrollingSubsystem>();
	if (PatrolingSubsystem)
	{
		PatrolingSubsystem->RegisterPatrolPath(this);
	}
}

int32 AAIPatrolPath::GetNextPatrolPoint(const int32 CurrentIndex, EDirectionType DirectionType) const
{
	if (PathPoints.Num() == 1)
	{
		return 0;
	}

	int32 NextIndex = CurrentIndex + (DirectionType == EDirectionType::Forward ? 1 : -1);

	const bool bIsInRange = NextIndex >= 0 && NextIndex < PathPoints.Num();
	if (bIsInRange)
	{
		return NextIndex;
	}

	if (PatrolType == EPatrolType::Circle)
	{
		return DirectionType == EDirectionType::Forward ? 0 : PathPoints.Num() - 1;
	}
	
	return NextIndex < 0 ? 1 : PathPoints.Num() - 2;
}

FVector AAIPatrolPath::GetPointLocation(int32 Index) const
{
	FVector Result {};
	if (Index < 0 || Index >= PathPoints.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("AAIPatrolPath::GetPointLocation: Index is out of bounds."));
		return Result;
	}

	return SceneComponent->GetComponentLocation() + PathPoints[Index];
}
