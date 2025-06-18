#pragma once

#include "Subsystems/ObjectSpawnSubsystem/SpawnSubsystemTypes.h"
#include "UObject/Object.h"
#include "Engine/EngineTypes.h"
#include "ObjectSpawnManager.generated.h"

class ABaseObjectSpawner;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnObjectSpawnedDelegate, ABaseObjectSpawner* /*ObjectSpawner*/, AActor* /*SpawnedActor*/);

UCLASS()
class SERIOUSTANK_API UObjectSpawnManager : public UObject
{
	GENERATED_BODY()
	
public:
	FOnObjectSpawnedDelegate OnObjectSpawnedDelegate;

public:
	void Initialize(const ESpawnObjectType InSpawnObjectType, const FObjectSpawnParameters& NewSpawnParameters, const UObject* SpawnerOwner = nullptr, FName InSpawnTag = NAME_None);
	void RegisterSpawner(ABaseObjectSpawner* SpawningActor);
	void UnregisterSpawner(ABaseObjectSpawner* SpawningActor);
	
	virtual void BeginDestroy() override;

	inline const UObject* GetSpawnOwner() const { return SpawnOwner; }
	inline void SetSpawnOwner(const UObject* InSpawnOwner) { SpawnOwner = InSpawnOwner; }

	inline FName GetSpawnTag() const { return SpawnTag; }
	inline void SetSpawnTag(FName InSpawnTag) { SpawnTag = InSpawnTag; }

	void SetIsAutoSpawnEnabled(bool bIsEnabled);
	void SetIsAutoDestroyEnabled(bool bIsEnabled);
	void SetIsSpawnLimited(bool bIsLimited);

	void SetSpawnTime(float NewSpawnTime);
	void SetDestroyTime(float NewDestroyTime);
	void SetMaxObjectsCount(int32 Count);

	AActor* SpawnRandomObject();
	TArray<AActor*> SpawnInAllAvailabeSpawners();

protected:
	virtual void OnObjectIsSpawned(ABaseObjectSpawner* SpawnVolume, AActor* SpawnedObject);

	virtual void OnDestroyTimerFinished(AActor* SpawnedObject);

private:
	AActor* SpawnRandomObject(int32 SpawnVolumeIndex);

	void OnSpawnerSetEnabled(ABaseObjectSpawner* SpawningActor, bool bIsEnabled);
	void OnSpawnedObjectDestroyed(ABaseObjectSpawner* SpawningActor, AActor* SpawnedObject);
	
	void SetSpawnTimerEnabled(bool bIsEnabled);


private:
	ESpawnObjectType SpawnObjectType;
	TObjectPtr<const UObject> SpawnOwner;
	FName SpawnTag;

	FObjectSpawnParameters SpawnParams;
	FTimerHandle SpawnTimerHandle;
	TSet<FTimerHandle> DestroyTimerHandles;

	TSet<ABaseObjectSpawner*> AvailableSpawnActors;
	int32 SpawnedObjectsCount = 0;
};
