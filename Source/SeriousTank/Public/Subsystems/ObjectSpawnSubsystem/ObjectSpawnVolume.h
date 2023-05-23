// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ObjectSpawnVolume.generated.h"

class AWeapon;
class UBoxComponent;

enum class ESpawnObjectType : uint8;

DECLARE_DELEGATE_TwoParams(FOnSpawnedObjectHandled, AObjectSpawnVolume*, AActor*);

UCLASS()
class SERIOUSTANK_API AObjectSpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:
	FOnSpawnedObjectHandled OnSpawnedObjectHandled;

protected:
	UPROPERTY(EditAnywhere)
	UBoxComponent* SpawningBoxComponent;

	UPROPERTY(EditAnywhere)
	TArray<ESpawnObjectType> SpawnObjectTypes;

	// TODO: Implement custom drop-box on base of DataTable
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AActor>> ClassesToSpawn;

	UPROPERTY(EditAnywhere)
	int32 MaxSpawnedObjects = 2;

private:
	int32 SpawnedObjectsCount = 0;
	
public:
	AObjectSpawnVolume();

	FORCEINLINE bool HasSpawnObjectType(ESpawnObjectType SpawnObjectType) { return SpawnObjectTypes.Contains(SpawnObjectType); }

	FORCEINLINE int32 GetSpawnedObjectsCount() const { return SpawnedObjectsCount; }
	FORCEINLINE int32 GetMaxObjectsCount() const { return MaxSpawnedObjects; }
	FORCEINLINE bool HasMaxObjectsCount() const { return SpawnedObjectsCount >= MaxSpawnedObjects; }
	
	AActor* SpawnRandomObject();

protected:
	// Function that allow to register actions on the spawned object,
	// after which the object will be considered as processed.
	virtual void RegisterObjectHandlers(AActor* NewObject);
	
	UFUNCTION()
	virtual void OnObjectHandled(AActor* HandledObject);

private:
	FVector GetRandomSpawnLocation() const;

};
