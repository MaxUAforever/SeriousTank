#include "Components/ST_SoldierMovementComponent.h"

#include "Core/ST_CoreTypes.h"
#include "Kismet/KismetMathLibrary.h"

UST_SoldierMovementComponent::UST_SoldierMovementComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UST_SoldierMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	DefaultMaxWalkSpeed = MaxWalkSpeed;
}

void UST_SoldierMovementComponent::SetSprintActive(bool bInIsSprintActive)
{
	bIsSprintActive = bInIsSprintActive;
	MaxWalkSpeed = bIsSprintActive ? MaxRunSpeed : DefaultMaxWalkSpeed;
}

void UST_SoldierMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	ECharacterMovingType NewMovingType = CalculateMovingType();
	if (MovingType != NewMovingType)
	{
		MovingType = NewMovingType;

		OnMovingTypeChanged.ExecuteIfBound(MovingType);
	}
}

ECharacterMovingType UST_SoldierMovementComponent::CalculateMovingType() const
{
	float GroundSpeed = UKismetMathLibrary::VSizeXY(Velocity);

	if (GroundSpeed <= 0)
	{
		return ECharacterMovingType::Standing;
	}
	else if (bIsSprintActive)
	{
		return ECharacterMovingType::Running;
	}

	return ECharacterMovingType::Walking;
}