// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BaseObjectSpawner.generated.h"

enum class ESpawnObjectType : uint8;

DECLARE_DELEGATE_TwoParams(FOnSpawnedObjectDestroyedDelegate, ABaseObjectSpawner*, AActor*);
DECLARE_DELEGATE_TwoParams(FOnSetSpawerEnabledDelegate, ABaseObjectSpawner*, bool);
DECLARE_DELEGATE_ThreeParams(FOnSpawnOwnerChanged, ABaseObjectSpawner*, UObject* /*OldOwner*/, UObject* /*NewOwner*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSpawnerSpawnedObject, AActor*);

UCLASS()
class SERIOUSTANK_API ABaseObjectSpawner : public AActor
{
	GENERATED_BODY()

public:
	FOnSpawnedObjectDestroyedDelegate OnSpawnedObjectDestroyedDelegate;
	FOnSetSpawerEnabledDelegate OnSetSpawerEnabledDelegate;
	FOnSpawnerSpawnedObject OnSpawnerSpawnedObjectDelegete;
	FOnSpawnOwnerChanged OnSpawnOwnerChangedDelegate;

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> SceneRootComponent;

	UPROPERTY(EditAnywhere)
	ESpawnObjectType SpawnObjectType;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AActor>> ClassesToSpawn;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UObject> SpawnOwner;

	UPROPERTY(EditAnywhere)
	FName SpawnTag;

	UPROPERTY(EditAnywhere)
	int32 MaxSpawnedObjects = 0;

	UPROPERTY(EditAnywhere)
	bool bIsEnabled = true;

private:
	int32 SpawnedObjectsCount = 0;

public:
	ABaseObjectSpawner();

protected:
	virtual void BeginPlay() override;

public:
	inline UObject* GetSpawnOwner() const { return SpawnOwner.Get(); }
	void SetSpawnOwner(UObject* InSpawnOwner);

	inline FName GetSpawnTag() const { return SpawnTag; }
	inline void SetSpawnTag(FName InSpawnTag) { SpawnTag = InSpawnTag; }

	inline ESpawnObjectType GetSpawnObjectType() const { return SpawnObjectType; }
	inline bool HasSpawnObjectType(ESpawnObjectType InSpawnObjectType) { return SpawnObjectType == InSpawnObjectType; }

	inline int32 GetSpawnedObjectsCount() const { return SpawnedObjectsCount; }
	inline int32 GetMaxObjectsCount() const { return MaxSpawnedObjects; }
	inline bool HasMaxObjectsCount() const { return MaxSpawnedObjects > 0 && SpawnedObjectsCount >= MaxSpawnedObjects; }

	inline bool IsEnabled() const { return bIsEnabled; }
	inline void SetIsEnabled(bool bInIsEnabled);

	bool CanSpawnObject() const;

	AActor* SpawnRandomObject();
	AActor* SpawnObject(TSubclassOf<AActor> ClassToSpawn);

protected:
	UFUNCTION()
	void OnSpanwedObjectDestroyed(AActor* HandledObject);

	virtual void OnObjectSpawned(AActor* NewObject) {};
	
	virtual FVector GetSpawnLocation() const;
};
