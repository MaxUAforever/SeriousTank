#pragma once

#include "SpawnSubsystemTypes.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSpawnSubsystem, Log, All);

UENUM()
enum class ESpawnObjectType : uint8
{
	Weapon,
	Ammo,
	Buff,
	Target,
	AIPawn,
	Custom
};

USTRUCT(BlueprintType)
struct FObjectSpawnParameters
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	bool bIsAutoSpawnEnabled = false;
	
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "bIsAutoSpawnEnabled"))
	bool bSpawnFromTheStart = false;

	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "bIsAutoSpawnEnabled", ClampMin = "0.0"))
	float SpawnTime = 10.f;

	UPROPERTY(EditDefaultsOnly)
	bool bIsAutoDestroyEnabled = false;
	
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "bIsAutoDestroyEnabled", ClampMin = "1.0"))
	float DestroyTime = 10.f;

	UPROPERTY(EditDefaultsOnly)
	bool bIsSpawnLimited = false;
	
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "bIsSpawnLimited", ClampMin = "1"))
	int32 MaxObjectsCount = 10;

	UPROPERTY(EditDefaultsOnly)
	bool bUseAllAvailableSpawners = false;
};

USTRUCT()
struct FObjectTypeSpawnParams
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	ESpawnObjectType SpawnObjectType = ESpawnObjectType::Custom;

	UPROPERTY(EditDefaultsOnly)
	FObjectSpawnParameters SpawnParameters;
};

