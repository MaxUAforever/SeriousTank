#include "Actors/Characters/Soldiers/ST_BaseSoldierAnimInstance.h"

#include "Components/ST_SoldierMovementComponent.h"
#include "Core/ST_CoreTypes.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"

void UST_BaseSoldierAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	MovingType = ECharacterMovingType::Standing;

	if (ACharacter* OwningCharacter = Cast<ACharacter>(TryGetPawnOwner()))
	{
		if (UST_SoldierMovementComponent* MovementComponent = Cast<UST_SoldierMovementComponent>(OwningCharacter->GetMovementComponent()))
		{
			MovementComponent->OnMovingTypeChanged.BindUObject(this, &ThisClass::OnMovementTypeChanged);
		}
	}
}
