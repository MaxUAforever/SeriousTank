// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BaseObjectSpawner.generated.h"

enum class ESpawnObjectType : uint8;

DECLARE_DELEGATE_TwoParams(FOnSpawnedObjectDestroyedDelegate, ABaseObjectSpawner*, AActor*);
DECLARE_DELEGATE_TwoParams(FOnSetSpawerEnabledDelegate, ABaseObjectSpawner*, bool);

UCLASS()
class SERIOUSTANK_API ABaseObjectSpawner : public AActor
{
	GENERATED_BODY()

public:
	FOnSpawnedObjectDestroyedDelegate OnSpawnedObjectDestroyedDelegate;
	FOnSetSpawerEnabledDelegate OnSetSpawerEnabledDelegate;
protected:
	UPROPERTY(EditAnywhere)
	TArray<ESpawnObjectType> SpawnObjectTypes;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AActor>> ClassesToSpawn;

	UPROPERTY(EditAnywhere)
	TObjectPtr<AActor> SpawnOwner;

	UPROPERTY(EditAnywhere)
	int32 MaxSpawnedObjects = 0;

	UPROPERTY(EditAnywhere)
	bool bIsEnabled = true;

private:
	int32 SpawnedObjectsCount = 0;

protected:
	virtual void BeginPlay() override;

public:
	FORCEINLINE bool HasSpawnObjectType(ESpawnObjectType SpawnObjectType) { return SpawnObjectTypes.Contains(SpawnObjectType); }
	FORCEINLINE const UObject* GetSpawnOwner() const { return SpawnOwner.Get(); }

	FORCEINLINE int32 GetSpawnedObjectsCount() const { return SpawnedObjectsCount; }
	FORCEINLINE int32 GetMaxObjectsCount() const { return MaxSpawnedObjects; }
	FORCEINLINE bool HasMaxObjectsCount() const { return MaxSpawnedObjects > 0 && SpawnedObjectsCount >= MaxSpawnedObjects; }

	FORCEINLINE bool IsEnabled() const { return bIsEnabled; }
	FORCEINLINE void SetIsEnabled(bool bInIsEnabled);

	bool CanSpawnObject() const;

	AActor* SpawnRandomObject();
	AActor* SpawnObject(TSubclassOf<AActor> ClassToSpawn);

protected:
	UFUNCTION()
	void OnSpanwedObjectDestroyed(AActor* HandledObject);

	virtual void OnObjectSpawned(AActor* NewObject) {};
	
	virtual FVector GetSpawnLocation() const;
};
