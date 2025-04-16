#pragma once

#include "Subsystems/WorldSubsystem.h"
#include "ObjectPoolSubsystem.generated.h"


/**
 * @brief Stores state information of an actor before puting it to the pool.
 */
USTRUCT()
struct FFreeObjectInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	TObjectPtr<AActor> Actor;

	UPROPERTY()
	bool bShouldTick = false;

	UPROPERTY()
	bool bIsCollisionEnabled = false;

	UPROPERTY()
	bool bShouldSimulatePhysics = false;

	UPROPERTY()
	TSet<UActorComponent*> ActiveComponents;
};

/**
 * @brief Pool of actors that are currently not in use.
 */
USTRUCT()
struct FFreeObjectPool
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	TArray<FFreeObjectInfo> ObjectsInfo;
};

/**
 * @brief Pool of actors that are currently active in the world.
 */
USTRUCT()
struct FActiveObjectPool
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	TSet<TObjectPtr<AActor>> Objects;
};

DECLARE_MULTICAST_DELEGATE(FOnObjectPoolSubsystemInitializedDelegate)

/**
 * @brief Subsystem that handles object pooling for actors to optimize spawning and destruction.
 *
 * Stores pre-instantiated actors and reuses them on request, avoiding the performance overhead of frequent spawning.
 */
UCLASS()
class OBJECTPOOLSUBSYSTEM_API UObjectPoolSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	/**
	 * @brief Initializes the pool based on settings.
	 *
	 * @param InWorld The world instance that is beginning play.
	 */
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	/**
	 * @brief Returns whether the pool subsystem has been initialized.
	 */
	bool IsInitialized() const { return bIsInitialized; }

	/**
	 * @brief Returns a delegate that is broadcast when the subsystem is initialized.
	 */
	FOnObjectPoolSubsystemInitializedDelegate& GetOnObjectPoolSubsystemInitializedDelegate() { return OnObjectPoolSubsystemInitializedDelegate; }

	/**
	 * @brief Spawns an actor from the pool or creates a new one if not available and allowed.
	 *
	 * @tparam T The actor class type.
	 * @param Class The class of the actor to spawn.
	 * @param Transform The transform where the actor should be spawned.
	 * @param SpawnParameters Additional parameters for actor spawning.
	 * @param bSpawnIfNotInPool If true, the actor will be spawned if not found in the pool.
	 * @return Pointer to the spawned actor, or nullptr if not spawned.
	 */
	template<class T>
	T* SpawnActor(UClass* Class, FTransform const& Transform, const FActorSpawnParameters& SpawnParameters = FActorSpawnParameters(), bool bSpawnIfNotInPool = true)
	{
		UWorld* World = GetWorld();
		if (!IsValid(World))
		{
			return nullptr;
		}

		if (bIsInitialized && FreeObjectsPoolMap.Contains(Class))
		{
			T* PoolActor = CastChecked<T>(GetActorFromPool(Class, &Transform, SpawnParameters), ECastCheckedType::NullAllowed);
			if (PoolActor != nullptr)
			{
				return PoolActor;
			}
		}
		
		if (bSpawnIfNotInPool)
		{
			return CastChecked<T>(World->SpawnActor(Class, &Transform, SpawnParameters), ECastCheckedType::NullAllowed);
		}

		return nullptr;
	}

	/**
	 * @brief Returns the specified actor to the pool, disabling it in the world.
	 *
	 * @param Actor The actor to return to the pool.
	 * @param bDestroyIfNotFromPool If true, destroys the actor if it wasn’t originally spawned from the pool.
	 */
	void ReturnToPool(AActor* Object, bool bDestroyIfNotFromPool = true);

protected:
	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;

private:
	/**
	 * @brief Retrieves an actor instance from the pool if available.
	 *
	 * @param Class The class of the actor to retrieve.
	 * @param UserTransformPtr Optional transform to apply when retrieving.
	 * @param SpawnParameters Parameters to apply to the actor.
	 * @return Pointer to the actor, or nullptr if not available.
	 */
	AActor* GetActorFromPool(UClass* Class, FTransform const* UserTransformPtr, const FActorSpawnParameters& SpawnParameters);

private:
	/** Map of free (inactive) actors by class. */
	UPROPERTY()
	TMap<TSubclassOf<AActor>, FFreeObjectPool> FreeObjectsPoolMap;

	/** Map of active (in-use) actors by class. */
	UPROPERTY()
	TMap<TSubclassOf<AActor>, FActiveObjectPool> ActiveObjectsPoolMap;

	/** Location where pooled actors are hidden from view. */
	FVector WorldPoolLocation = { 0.f, 0.f, -1000.f };

	/** Whether the pool subsystem is initialized and ready. */
	bool bIsInitialized = false;

	/** Delegate broadcast when the pool subsystem finishes initialization. */
	FOnObjectPoolSubsystemInitializedDelegate OnObjectPoolSubsystemInitializedDelegate;
};
