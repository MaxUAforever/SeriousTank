#pragma once

#include "GameFramework/CharacterMovementComponent.h"
#include "ST_SoldierMovementComponent.generated.h"

enum class ECharacterMovingType : uint8;

UCLASS()
class SERIOUSTANK_API UST_SoldierMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FOnMovingTypeChanged, ECharacterMovingType)
	FOnMovingTypeChanged OnMovingTypeChanged;

protected:
	/** The maximum ground speed when running. Also determines maximum lateral speed when falling. */
	UPROPERTY(Category="Character Movement: Walking", EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0", UIMin="0", ForceUnits="cm/s"))
	float MaxRunSpeed;

	UPROPERTY(BlueprintReadOnly)
	ECharacterMovingType MovingType;

private:
	float DefaultMaxWalkSpeed;
	bool bIsSprintActive;

public:
	UST_SoldierMovementComponent(const FObjectInitializer& ObjectInitializer);
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

public:
	FORCEINLINE bool IsSprintActive() const { return bIsSprintActive; }

	/** Set MaxWalkSpeed to MaxRunSpeed or DefaultMaxWalkSpeed depends on function parameter. */
	UFUNCTION(BlueprintCallable, Category = "Pawn|Components|CharacterMovement")
	void SetSprintActive(bool bInIsSprintActive);

private:
	ECharacterMovingType CalculateMovingType() const;
};
