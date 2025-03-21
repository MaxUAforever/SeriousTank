// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Subsystems/WorldSubsystem.h"

#include "Subsystems/ObjectSpawnSubsystem/ObjectSpawnManager.h"
#include "Subsystems/ObjectSpawnSubsystem/SpawnSubsystemTypes.h"
#include "ObjectSpawnSubsystem.generated.h"

USTRUCT()
struct FObjectSpawnManagerInfo
{
	GENERATED_BODY()

public:
	friend uint32 GetTypeHash(const FObjectSpawnManagerInfo& Settings);

	bool operator==(const FObjectSpawnManagerInfo& Other) const
	{
		return SpawnObjectType == Other.SpawnObjectType && SpawnManagerOwner == Other.SpawnManagerOwner;
	}

public:
	UPROPERTY()
	ESpawnObjectType SpawnObjectType;

	UPROPERTY()
	TObjectPtr<const UObject> SpawnManagerOwner;
};

UCLASS()
class SERIOUSTANK_API UObjectSpawnSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	TMap<FObjectSpawnManagerInfo, TObjectPtr<UObjectSpawnManager>> ObjectSpawnManagers;

public:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	const UObjectSpawnManager* AddObjectSpawnManager(ESpawnObjectType SpawnObjectType, const FObjectSpawnParameters& SpawnParameters, UObject* InSpawnManagerOwner = nullptr);
	const UObjectSpawnManager* GetObjectSpawnManager(ESpawnObjectType SpawnObjectType, const UObject* InSpawnManagerOwner = nullptr);
	const UObjectSpawnManager* FindOrAddObjectSpawnManager(ESpawnObjectType SpawnObjectType, UObject* InSpawnManagerOwner = nullptr);

	void SetupSpawnManager(ESpawnObjectType SpawnObjectType, const FObjectSpawnParameters& SpawnParameters, const UObject* InSpawnManagerOwner = nullptr);

	bool SpawnObject(ESpawnObjectType SpawnObjectType, const UObject* InSpawnManagerOwner = nullptr);
	FOnObjectSpawnedDelegate* GetManagerObjectSpawnedDelegate(ESpawnObjectType SpawnObjectType, const UObject* InSpawnManagerOwner = nullptr);

private:
	UObjectSpawnManager* FindObjectSpawnManager(ESpawnObjectType SpawnObjectType, const UObject* InSpawnManagerOwner);
};
