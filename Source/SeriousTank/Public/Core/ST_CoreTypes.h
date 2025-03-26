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

	UPROPERTY(EditDefaultsOnly, Category = "Game")
	bool bShouldGameStopAfterDeath = true;
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
	TSubclassOf<AST_BaseWeapon> MainWeaponClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AST_BaseWeapon> SecondWeaponClass;
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
