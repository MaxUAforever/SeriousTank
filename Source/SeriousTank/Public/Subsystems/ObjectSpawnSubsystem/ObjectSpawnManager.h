#pragma once

#include "Subsystems/ObjectSpawnSubsystem/SpawnSubsystemTypes.h"
#include "UObject/Object.h"
#include "Engine/EngineTypes.h"
#include "ObjectSpawnManager.generated.h"

class ABaseObjectSpawner;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnObjectSpawnedDelegate, ABaseObjectSpawner* /*ObjectSpawner*/, AActor* /*SpawnedActor*/);
DECLARE_DELEGATE_ThreeParams(FOnSpawnerOwnerChanged, ABaseObjectSpawner*, UObject* /*OldOwner*/, UObject* /*NewOwner*/);

UCLASS()
class SERIOUSTANK_API UObjectSpawnManager : public UObject
{
	GENERATED_BODY()
	
public:
	FOnObjectSpawnedDelegate OnObjectSpawnedDelegate;
	FOnSpawnerOwnerChanged OnSpawnerOwnerChangedDelegate;

private:
	ESpawnObjectType SpawnObjectType;
	TObjectPtr<const UObject> SpawnOwner;

	FObjectSpawnParameters SpawnParams;
	FTimerHandle SpawnTimerHandle;
	TSet<FTimerHandle> DestroyTimerHandles;

	TSet<ABaseObjectSpawner*> AvailableSpawnActors;
	int32 SpawnedObjectsCount = 0;

public:
	void Initialize(const ESpawnObjectType InSpawnObjectType, const FObjectSpawnParameters& NewSpawnParameters, const UObject* SpawnerOwner = nullptr);
	void RegisterSpawner(ABaseObjectSpawner* SpawningActor);
	void UnregisterSpawner(ABaseObjectSpawner* SpawningActor);
	
	virtual void BeginDestroy() override;

	void SetIsAutoSpawnEnabled(bool bIsEnabled);
	void SetIsAutoDestroyEnabled(bool bIsEnabled);
	void SetIsSpawnLimited(bool bIsLimited);

	void SetSpawnTime(float NewSpawnTime);
	void SetDestroyTime(float NewDestroyTime);
	void SetMaxObjectsCount(int32 Count);

	AActor* SpawnRandomObject();

protected:
	virtual void OnObjectIsSpawned(ABaseObjectSpawner* SpawnVolume, AActor* SpawnedObject);

	virtual void OnDestroyTimerFinished(AActor* SpawnedObject);

private:
	AActor* SpawnRandomObject(int32 SpawnVolumeIndex);

	void OnSpawnerSetEnabled(ABaseObjectSpawner* SpawningActor, bool bIsEnabled);
	void OnSpawnedObjectDestroyed(ABaseObjectSpawner* SpawningActor, AActor* SpawnedObject);
	void OnSpawnerOwnerChanged(ABaseObjectSpawner* SpawningActor, UObject* OldOwner, UObject* NewOwner);

	void SetSpawnTimerEnabled(bool bIsEnabled);

	void UpdateAvailableSpawners();
};
