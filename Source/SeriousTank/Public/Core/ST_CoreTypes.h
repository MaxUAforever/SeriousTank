#pragma once

#include "Templates/SubclassOf.h"

#include "UObject/Object.h"
#include "ST_CoreTypes.generated.h"

class AST_ShootTarget;
class AST_BaseVehicle;
class ABaseWeapon;

USTRUCT(BlueprintType)
struct FGameData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, Category="Game", meta = (ClampMin = "0", ClampMax = "5"))
	int32 PreStartCountdownTime = 3;

	UPROPERTY(EditDefaultsOnly, Category="Game", meta = (ClampMin = "1", ClampMax = "500"))
	int32 StartTime = 50;

	UPROPERTY(EditDefaultsOnly, Category="Game", meta = (ClampMin = "1", ClampMax = "500"))
	float AddedTimeForTarget = 5.f;

	UPROPERTY(EditDefaultsOnly, Category="Game", meta = (ClampMin = "1", ClampMax = "500"))
	int32 AddedScoreForTarget = 1;

	UPROPERTY(EditDefaultsOnly, Category = "Game")
	TSubclassOf<AST_ShootTarget> ShootingTargetClass;
};

USTRUCT(BlueprintType)
struct FGameplayLevelInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly)
	FString LevelName;

	UPROPERTY(EditDefaultsOnly)
	FString LevelCaption;
};

USTRUCT(BlueprintType)
struct FVehicleInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<AST_BaseVehicle> VehicleClass;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<ABaseWeapon>> WeaponClasses;
    
public:
    FORCEINLINE bool operator==(const FVehicleInfo& Other) const
    {
        return VehicleClass == Other.VehicleClass;
    }
};

UENUM(BlueprintType)
enum class EMovingType : uint8
{
	ForceMoving,
	InertiaMoving,
	Braking,
	RotatingInPlace,
	Standing
};
