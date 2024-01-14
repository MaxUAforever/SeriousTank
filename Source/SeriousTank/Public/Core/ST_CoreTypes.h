#pragma once

#include "Actors/Pawns/ST_BaseVehicle.h"
#include "Templates/SubclassOf.h"
#include "UObject/Object.h"

#include "ST_CoreTypes.generated.h"

class AST_BaseWeapon;
class AST_ShootTarget;
class AST_TargetSpawningVolume;

USTRUCT(BlueprintType)
struct FBaseGameData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, Category="Game", meta = (ClampMin = "0", ClampMax = "5"))
	int32 PreStartCountdownTime = 3;
};

USTRUCT(BlueprintType)
struct FTargetShootingGameData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, Category="Game", meta = (ClampMin = "1", ClampMax = "500"))
	int32 StartTime = 50;

	UPROPERTY(EditDefaultsOnly, Category="Game", meta = (ClampMin = "1", ClampMax = "500"))
	float AddedTimeForTarget = 5.f;

	UPROPERTY(EditDefaultsOnly, Category="Game", meta = (ClampMin = "1", ClampMax = "500"))
	int32 AddedScoreForTarget = 1;
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
	TArray<TSubclassOf<AST_BaseWeapon>> WeaponClasses;
    
public:
    FORCEINLINE bool operator==(const FVehicleInfo& Other) const
    {
        return VehicleClass == Other.VehicleClass;
    }
};

USTRUCT(BlueprintType)
struct FSoldierInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<AST_BaseWeapon> WeaponClass;
};

UENUM(BlueprintType)
enum class EWeaponOwnerType : uint8
{
	Soldier,
	Vehicle
};

UENUM(BlueprintType)
enum class ECharacterMovingType : uint8
{
	Standing,
	Walking,
	Running
};

UENUM(BlueprintType)
enum class ECharacterTurnSide : uint8
{
	None,
	Left,
	Right
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
