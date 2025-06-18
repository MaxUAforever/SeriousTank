// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Subsystems/WorldSubsystem.h"

#include "Subsystems/ObjectSpawnSubsystem/ObjectSpawnManager.h"
#include "Subsystems/ObjectSpawnSubsystem/SpawnSubsystemTypes.h"
#include "ObjectSpawnSubsystem.generated.h"

class ABaseObjectSpawner;

USTRUCT()
struct FObjectSpawnManagerList
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	TArray<TObjectPtr<UObjectSpawnManager>> ObjectSpawnManagers;
};

UCLASS()
class SERIOUSTANK_API UObjectSpawnSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	const UObjectSpawnManager* AddObjectSpawnManager(ESpawnObjectType SpawnObjectType, const FObjectSpawnParameters& SpawnParameters, UObject* InSpawnManagerOwner = nullptr, FName InTag = NAME_None);
	const UObjectSpawnManager* GetObjectSpawnManager(ESpawnObjectType SpawnObjectType, const UObject* InSpawnManagerOwner = nullptr, FName InTag = NAME_None);
	const UObjectSpawnManager* FindOrAddObjectSpawnManager(ESpawnObjectType SpawnObjectType, UObject* InSpawnManagerOwner = nullptr, FName InTag = NAME_None);

	void SetupSpawnManager(ESpawnObjectType SpawnObjectType, const FObjectSpawnParameters& SpawnParameters, const UObject* InSpawnManagerOwner = nullptr, FName InTag = NAME_None);

	AActor* SpawnObject(ESpawnObjectType SpawnObjectType, const UObject* InSpawnManagerOwner = nullptr, FName InTag = NAME_None);
	bool SpawnInAllAvailableSpawners(ESpawnObjectType SpawnObjectType, const UObject* InSpawnManagerOwner = nullptr, FName InTag = NAME_None);

	FOnObjectSpawnedDelegate* GetManagerObjectSpawnedDelegate(ESpawnObjectType SpawnObjectType, const UObject* InSpawnManagerOwner = nullptr, FName InTag = NAME_None);

	void RegisterSpawner(ABaseObjectSpawner* ObjectSpawner);
	void UnregisterSpawner(ABaseObjectSpawner* ObjectSpawner);

	void AddDefaultSpawnParameters(ESpawnObjectType SpawnObjectType, const FObjectSpawnParameters& SpawnParameters);

private:
	UObjectSpawnManager* FindObjectSpawnManager_Internal(ESpawnObjectType SpawnObjectType, const UObject* InSpawnManagerOwner, FName InTag);
	
	UObjectSpawnManager* AddObjectSpawnManager_Internal(ESpawnObjectType SpawnObjectType, const FObjectSpawnParameters& SpawnParameters, UObject* InSpawnManagerOwner = nullptr, FName InTag = NAME_None);
	UObjectSpawnManager* FindOrAddObjectSpawnManager_Internal(ESpawnObjectType SpawnObjectType, UObject* InSpawnManagerOwner, FName InTag);

	void OnObjectSpawned(AActor* SpawnedActor, ESpawnObjectType SpawnObjectType, const UObject* InSpawnManagerOwner, UObjectSpawnManager* SpawnManager);
	void OnSpawnerOwnerChanged(ABaseObjectSpawner* ObjectSpawner, UObject* OldOwner, UObject* NewOwner);

private:
	UPROPERTY()
	TMap<ESpawnObjectType, FObjectSpawnManagerList> ObjectSpawnManagers;

	UPROPERTY()
	TMap<ESpawnObjectType, FObjectSpawnParameters> DefaultSpawnParameters;
};
