#include "Subsystems/ObjectSpawnSubsystem/ObjectSpawnSubsystem.h"

#include "Subsystems/ObjectSpawnSubsystem/ObjectSpawnManager.h"
#include "Engine/EngineBaseTypes.h"
#include "Engine/World.h"

bool UObjectSpawnSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	UWorld* World = Cast<UWorld>(Outer);

	return Super::ShouldCreateSubsystem(Outer) && World && !World->IsNetMode(NM_Client);
}

UObjectSpawnManager* UObjectSpawnSubsystem::AddObjectSpawnManager(ESpawnObjectType SpawnObjectType, const FObjectSpawnParameters& SpawnParameters)
{
	if (ObjectSpawnManagers.Contains(SpawnObjectType))
	{
		UE_LOG(LogTemp, Warning, TEXT("ObjectSpawnSubsystem::AddObjectSpawnType: ObjectSpawnManager already exists."));
		return nullptr;
	}

	UObjectSpawnManager* SpawnManager = NewObject<UObjectSpawnManager>(this);
	SpawnManager->Initialize(SpawnObjectType, SpawnParameters);

	ObjectSpawnManagers.Add(SpawnObjectType, SpawnManager);

	return SpawnManager;
}

UObjectSpawnManager* UObjectSpawnSubsystem::GetObjectSpawnManager(ESpawnObjectType SpawnObjectType)
{
	UObjectSpawnManager** SpawnManagerPtr = ObjectSpawnManagers.Find(SpawnObjectType);

	return SpawnManagerPtr ? *SpawnManagerPtr : nullptr;
}
