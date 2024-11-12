// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "AIPatrolPath.generated.h"

UENUM(BlueprintType)
enum class EPatrolType : uint8
{
	Circle,
	TwoWay
};

UENUM(BlueprintType)
enum class EDirectionType : uint8
{
	Forward,
	Backward
};

class USceneComponent;

UCLASS()
class AIPATROLLINGSUBSYSTEM_API AAIPatrolPath : public AActor
{
	GENERATED_BODY()
	
public:
	AAIPatrolPath();

protected:
	virtual void BeginPlay() override;

public:
	FORCEINLINE const TArray<FVector>& GetPathPoints() const { return PathPoints; }
	FORCEINLINE EPatrolType GetPatrolType() const { return PatrolType; }
	FORCEINLINE EDirectionType GetInitialDirectionType() const { return InitialDirectionType; }
	FORCEINLINE int32 GetMaxPatrollingAgents() const { return MaxPatrollingAgents; }
	FORCEINLINE int32 GetPriority() const { return Priority; }

	int32 GetNextPatrolPoint(const int32 CurrentIndex, EDirectionType DirectionType) const;

	FVector GetPointLocation(int32 Index) const;

protected:
	UPROPERTY(EditDefaultsOnly)
	USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(MakeEditWidget="true"))
	TArray<FVector> PathPoints;

	UPROPERTY(EditAnywhere)
	EPatrolType PatrolType = EPatrolType::Circle;

	UPROPERTY(EditAnywhere)
	EDirectionType InitialDirectionType = EDirectionType::Forward;

	UPROPERTY(EditAnywhere, meta=(ClampMin="0"))
	int32 MaxPatrollingAgents = 2;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	int32 Priority = 0;
};
