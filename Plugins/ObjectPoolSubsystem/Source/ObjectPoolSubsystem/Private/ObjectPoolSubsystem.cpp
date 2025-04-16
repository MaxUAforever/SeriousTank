#include "ObjectPoolSubsystem.h"

#include "ObjectPoolSubsystemSettings.h"

void UObjectPoolSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	const UObjectPoolSubsystemSettings* ObjectPoolSubsystemSettings = GetDefault<UObjectPoolSubsystemSettings>();
	if (!IsValid(ObjectPoolSubsystemSettings))
	{
		return;
	}

	FString CurrentMapName = InWorld.GetMapName();
	CurrentMapName.RemoveFromStart(InWorld.StreamingLevelsPrefix);

	if (ObjectPoolSubsystemSettings->CommonRestrictedMaps.Contains(CurrentMapName))
	{
		bIsInitialized = true;
		OnObjectPoolSubsystemInitializedDelegate.Broadcast();

		return;
	}

	for (const FObjectPoolInfo& ObjectPoolInfo : ObjectPoolSubsystemSettings->ObjectsPoolInfo)
	{
		const bool bIsMapRestricted = ObjectPoolInfo.RestrictedMaps.Contains(CurrentMapName);
		const bool bIsMapAllowed = ObjectPoolInfo.AllowedMaps.IsEmpty() || ObjectPoolInfo.AllowedMaps.Contains(CurrentMapName);
		if (!bIsMapAllowed || bIsMapRestricted)
		{
			continue;
		}

		FActiveObjectPool& ActiveObjectPool = ActiveObjectsPoolMap.Emplace(ObjectPoolInfo.ObjectClass, {});
		FFreeObjectPool& ObjectPool = FreeObjectsPoolMap.Emplace(ObjectPoolInfo.ObjectClass, {});
		ObjectPool.ObjectsInfo.Reserve(ObjectPoolInfo.ObjectsAmount);

		for (int Index = 0; Index < ObjectPoolInfo.ObjectsAmount; ++Index)
		{
			static const FTransform DefaultSpawnTransform = FTransform(FRotator{}, WorldPoolLocation, FVector{ 1.f });
			AActor* SpawnedActor = InWorld.SpawnActor(ObjectPoolInfo.ObjectClass, &DefaultSpawnTransform);
			if (!IsValid(SpawnedActor))
			{
				continue;
			}

			ActiveObjectPool.Objects.Add(SpawnedActor);
			ReturnToPool(SpawnedActor);

#if WITH_EDITOR
			SpawnedActor->SetFolderPath("_ObjectsPool");
#endif
		}
	}

	bIsInitialized = true;
	OnObjectPoolSubsystemInitializedDelegate.Broadcast();
}

AActor* UObjectPoolSubsystem::GetActorFromPool(UClass* Class, FTransform const* UserTransformPtr, const FActorSpawnParameters& SpawnParameters)
{
	FFreeObjectPool* FreeObjectsPool = FreeObjectsPoolMap.Find(Class);
	if (FreeObjectsPool == nullptr || FreeObjectsPool->ObjectsInfo.IsEmpty() || UserTransformPtr == nullptr)
	{
		return nullptr;
	}

	FFreeObjectInfo FreeObjectInfo = FreeObjectsPool->ObjectsInfo.Pop(false);
	AActor* Actor = FreeObjectInfo.Actor;
	if (!IsValid(Actor))
	{
		return nullptr;
	}

	Actor->SetActorTransform(*UserTransformPtr);
	Actor->SetOwner(SpawnParameters.Owner);
	Actor->SetInstigator(SpawnParameters.Instigator);
	Actor->SetActorTickEnabled(FreeObjectInfo.bShouldTick);
	Actor->SetActorEnableCollision(FreeObjectInfo.bIsCollisionEnabled);
	
	if (UPrimitiveComponent* PrimitiveRootComponent = Cast<UPrimitiveComponent>(Actor->GetRootComponent()))
	{
		PrimitiveRootComponent->SetSimulatePhysics(FreeObjectInfo.bShouldSimulatePhysics);
	}

	for (UActorComponent* Component : FreeObjectInfo.ActiveComponents)
	{
		if (IsValid(Component))
		{
			Component->Activate();
		}
	}

	if (FActiveObjectPool* ActiveObjectPool = ActiveObjectsPoolMap.Find(Class))
	{
		ActiveObjectPool->Objects.Add(Actor);
	}

	Actor->SetActorHiddenInGame(false);

	return Actor;
}

void UObjectPoolSubsystem::ReturnToPool(AActor* Actor, bool bDestroyIfNotFromPool)
{
	TSubclassOf<AActor> ObjectClass = Actor->GetClass();
	FActiveObjectPool* ActiveObjectPool = ActiveObjectsPoolMap.Find(ObjectClass);
	if (ActiveObjectPool == nullptr || !ActiveObjectPool->Objects.Contains(Actor))
	{
		if (bDestroyIfNotFromPool)
		{
			Actor->Destroy();
		}

		return;
	}

	ActiveObjectPool->Objects.Remove(Actor);
	FFreeObjectPool* FreeObjectsPool = FreeObjectsPoolMap.Find(ObjectClass);
	if (FreeObjectsPool == nullptr)
	{
		return;
	}

	bool bShouldSimulatePhysics = false;
	if (UPrimitiveComponent* PrimitiveRootComponent = Cast<UPrimitiveComponent>(Actor->GetRootComponent()))
	{
		bShouldSimulatePhysics = PrimitiveRootComponent->IsSimulatingPhysics();
		PrimitiveRootComponent->SetSimulatePhysics(false);
	}

	FFreeObjectInfo FreeObjectInfo = { Actor, Actor->IsActorTickEnabled(), Actor->GetActorEnableCollision(), bShouldSimulatePhysics, {}};
	
	Actor->SetActorHiddenInGame(true);
	Actor->SetActorTickEnabled(false);
	Actor->SetActorEnableCollision(false);
	Actor->SetActorLocation(WorldPoolLocation);

	TArray<UActorComponent*> Components;
	Actor->GetComponents(Components);

	for (UActorComponent* Component : Components)
	{
		if (IsValid(Component) && Component->IsActive())
		{
			FreeObjectInfo.ActiveComponents.Add(Component);
			Component->Deactivate();
		}
	}

	FreeObjectsPool->ObjectsInfo.Add(FreeObjectInfo);
}

bool UObjectPoolSubsystem::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}
