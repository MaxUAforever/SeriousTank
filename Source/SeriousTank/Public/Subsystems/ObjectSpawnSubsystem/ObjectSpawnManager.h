#pragma once

#include "Subsystems/ObjectSpawnSubsystem/SpawnSubsystemTypes.h"
#include "UObject/Object.h"
#include "Engine/EngineTypes.h"
#include "ObjectSpawnManager.generated.h"

class AObjectSpawnVolume;

UCLASS()
class SERIOUSTANK_API UObjectSpawnManager : public UObject
{
	GENERATED_BODY()
	
private:
	FObjectSpawnParameters SpawnParams;
	FTimerHandle SpawnTimerHandle;
	TSet<FTimerHandle> DestroyTimerHandles;

	// TODO: Create common interface for spawn actors and add ObjectSpawnPoint.
	TArray<AObjectSpawnVolume*> AvailableSpawnVolumes;
	int32 SpawnedObjectsCount = 0;

public:
	void Initialize(const ESpawnObjectType SpawnObjectType, const FObjectSpawnParameters& NewSpawnParameters);
	virtual void BeginDestroy() override;

	void SetIsAutoSpawnEnabled(bool bIsEnabled);
	void SetIsAutoDestroyEnabled(bool bIsEnabled);
	void SetIsSpawnLimited(bool bIsLimited);

	void SetSpawnTime(float NewSpawnTime);
	void SetDestroyTime(float NewDestroyTime);
	void SetMaxObjectsCount(int32 Count);

	bool SpawnRandomObject();
	bool SpawnRandomObject(int32 SpawnVolumeIndex);

protected:
	virtual void OnObjectIsSpawned(AObjectSpawnVolume* SpawnVolume, AActor* SpawnedObject);

	virtual void OnDestroyTimerFinished(AActor* SpawnedObject);

private:
	void OnSpawnedObjectHandled(AObjectSpawnVolume* SpawnVolume, AActor* SpawnedObject);

	void SetSpawnTimerEnabled(bool bIsEnabled);
};
