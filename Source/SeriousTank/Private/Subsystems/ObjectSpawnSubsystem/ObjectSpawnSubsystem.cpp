#include "Subsystems/ObjectSpawnSubsystem/ObjectSpawnSubsystem.h"

#include "Engine/EngineBaseTypes.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "Subsystems/ObjectSpawnSubsystem/BaseObjectSpawner.h"
#include "Subsystems/ObjectSpawnSubsystem/ObjectSpawnManager.h"

bool UObjectSpawnSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	UWorld* World = Cast<UWorld>(Outer);

	return Super::ShouldCreateSubsystem(Outer) && World && !World->IsNetMode(NM_Client);
}

const UObjectSpawnManager* UObjectSpawnSubsystem::AddObjectSpawnManager(ESpawnObjectType SpawnObjectType, const FObjectSpawnParameters& SpawnParameters, UObject* InSpawnManagerOwner, FName InTag)
{
	return AddObjectSpawnManager_Internal(SpawnObjectType, SpawnParameters, InSpawnManagerOwner, InTag);
}

const UObjectSpawnManager* UObjectSpawnSubsystem::GetObjectSpawnManager(ESpawnObjectType SpawnObjectType, const UObject* InSpawnManagerOwner, FName InTag)
{
	return FindObjectSpawnManager_Internal(SpawnObjectType, InSpawnManagerOwner, InTag);
}

const UObjectSpawnManager* UObjectSpawnSubsystem::FindOrAddObjectSpawnManager(ESpawnObjectType SpawnObjectType, UObject* InSpawnManagerOwner, FName InTag)
{
	return FindOrAddObjectSpawnManager_Internal(SpawnObjectType, InSpawnManagerOwner, InTag);
}

void UObjectSpawnSubsystem::SetupSpawnManager(ESpawnObjectType SpawnObjectType, const FObjectSpawnParameters& SpawnParameters, const UObject* InSpawnManagerOwner, FName InTag)
{
	UObjectSpawnManager* SpawnManager = FindObjectSpawnManager_Internal(SpawnObjectType, InSpawnManagerOwner, InTag);
	if (!IsValid(SpawnManager))
	{
		UE_LOG(LogTemp, Warning, TEXT("ObjectSpawnSubsystem::SetupSpawnManager: Trying to setup not existing SpawnManager"));
		return;
	}

	SpawnManager->SetSpawnTime(SpawnParameters.SpawnTime);
	SpawnManager->SetDestroyTime(SpawnParameters.DestroyTime);
	SpawnManager->SetMaxObjectsCount(SpawnParameters.MaxObjectsCount);
	
	SpawnManager->SetIsSpawnLimited(SpawnParameters.bIsSpawnLimited);
	SpawnManager->SetIsAutoSpawnEnabled(SpawnParameters.bIsAutoSpawnEnabled);
	SpawnManager->SetIsAutoDestroyEnabled(SpawnParameters.bIsAutoDestroyEnabled);
}

AActor* UObjectSpawnSubsystem::SpawnObject(ESpawnObjectType SpawnObjectType, const UObject* InSpawnManagerOwner, FName InTag)
{
	UObjectSpawnManager* SpawnManager = FindObjectSpawnManager_Internal(SpawnObjectType, InSpawnManagerOwner, InTag);
	if (!IsValid(SpawnManager))
	{
		UE_LOG(LogTemp, Warning, TEXT("ObjectSpawnSubsystem::SpawnObject: Trying to spawn object with not existing SpawnManager"));
		return nullptr;
	}

	AActor* SpawnedActor = SpawnManager->SpawnRandomObject();
	OnObjectSpawned(SpawnedActor, SpawnObjectType, InSpawnManagerOwner, SpawnManager);

	return SpawnedActor;
}

bool UObjectSpawnSubsystem::SpawnInAllAvailableSpawners(ESpawnObjectType SpawnObjectType, const UObject* InSpawnManagerOwner, FName InTag)
{
	UObjectSpawnManager* SpawnManager = FindObjectSpawnManager_Internal(SpawnObjectType, InSpawnManagerOwner, InTag);
	if (!IsValid(SpawnManager))
	{
		UE_LOG(LogTemp, Warning, TEXT("ObjectSpawnSubsystem::SpawnInAllAvailableSpawners: Trying to spawn in not existing SpawnManager"));
		return false;
	}

	TArray<AActor*> SpawnedActors = SpawnManager->SpawnInAllAvailabeSpawners();
	for (AActor* SpawnedActor : SpawnedActors)
	{
		OnObjectSpawned(SpawnedActor, SpawnObjectType, InSpawnManagerOwner, SpawnManager);
	}

	return SpawnedActors.Num() > 0;
}

FOnObjectSpawnedDelegate* UObjectSpawnSubsystem::GetManagerObjectSpawnedDelegate(ESpawnObjectType SpawnObjectType, const UObject* InSpawnManagerOwner, FName InTag)
{
	UObjectSpawnManager* SpawnManager = FindObjectSpawnManager_Internal(SpawnObjectType, InSpawnManagerOwner, InTag);
	if (!IsValid(SpawnManager))
	{
		UE_LOG(LogTemp, Warning, TEXT("ObjectSpawnSubsystem::GetManagerObjectSpawnedDelegate: Trying to get delegate with not existing SpawnManager"));
		return nullptr;
	}

	return &SpawnManager->OnObjectSpawnedDelegate;
}

void UObjectSpawnSubsystem::RegisterSpawner(ABaseObjectSpawner* ObjectSpawner)
{
	if (!IsValid(ObjectSpawner))
	{
		UE_LOG(LogTemp, Warning, TEXT("ObjectSpawnSubsystem::RegisterSpawner: Trying to register invalid ObjectSpawner"));
		return;
	}

	ObjectSpawner->OnSpawnOwnerChangedDelegate.BindUObject(this, &ThisClass::OnSpawnerOwnerChanged);

	UObjectSpawnManager* ObjectSpawnManager = FindOrAddObjectSpawnManager_Internal(ObjectSpawner->GetSpawnObjectType(), ObjectSpawner->GetSpawnOwner(), ObjectSpawner->GetSpawnTag());
	ObjectSpawnManager->RegisterSpawner(ObjectSpawner);
}

void UObjectSpawnSubsystem::UnregisterSpawner(ABaseObjectSpawner* ObjectSpawner)
{
	if (!IsValid(ObjectSpawner))
	{
		UE_LOG(LogTemp, Warning, TEXT("ObjectSpawnSubsystem::UnregisterSpawner: Trying to unregister invalid ObjectSpawner"));
		return;
	}

	ObjectSpawner->OnSpawnOwnerChangedDelegate.Unbind();

	UObjectSpawnManager* ObjectSpawnManager = FindObjectSpawnManager_Internal(ObjectSpawner->GetSpawnObjectType(), ObjectSpawner->GetSpawnOwner(), ObjectSpawner->GetSpawnTag());
	ObjectSpawnManager->UnregisterSpawner(ObjectSpawner);
}

void UObjectSpawnSubsystem::AddDefaultSpawnParameters(ESpawnObjectType SpawnObjectType, const FObjectSpawnParameters& SpawnParameters)
{
	DefaultSpawnParameters.Emplace(SpawnObjectType, SpawnParameters);
}

UObjectSpawnManager* UObjectSpawnSubsystem::FindObjectSpawnManager_Internal(ESpawnObjectType SpawnObjectType, const UObject* InSpawnManagerOwner, FName InTag)
{
	const UObject* SpawnManagerOwner = InSpawnManagerOwner ? InSpawnManagerOwner : GetWorld();
	if (!IsValid(SpawnManagerOwner))
	{
		UE_LOG(LogTemp, Warning, TEXT("ObjectSpawnSubsystem::FindObjectSpawnManager: Trying to get SpawnManager with invalid Owner"));
		return nullptr;
	}

	FObjectSpawnManagerList* SpawnManagersList = ObjectSpawnManagers.Find(SpawnObjectType);
	if (SpawnManagersList == nullptr)
	{
		return nullptr;
	}

	for (UObjectSpawnManager* ObjectSpawnManager : SpawnManagersList->ObjectSpawnManagers)
	{
		if (IsValid(ObjectSpawnManager) && (ObjectSpawnManager->GetSpawnOwner() == InSpawnManagerOwner) && (ObjectSpawnManager->GetSpawnTag() == InTag))
		{
			return ObjectSpawnManager;
		}
	}

	return nullptr;
}

UObjectSpawnManager* UObjectSpawnSubsystem::AddObjectSpawnManager_Internal(ESpawnObjectType SpawnObjectType, const FObjectSpawnParameters& SpawnParameters, UObject* InSpawnManagerOwner, FName InTag)
{
	UObject* SpawnManagerOwner = InSpawnManagerOwner ? InSpawnManagerOwner : GetWorld();
	if (!IsValid(SpawnManagerOwner))
	{
		UE_LOG(LogTemp, Warning, TEXT("ObjectSpawnSubsystem::AddObjectSpawnManager: Trying to add SpawnManager with invalid Owner"));
		return nullptr;
	}

	TObjectPtr<UObjectSpawnManager> CurrentObjectSpawnManager = FindObjectSpawnManager_Internal(SpawnObjectType, InSpawnManagerOwner, InTag);
	if (!IsValid(CurrentObjectSpawnManager))
	{
		FObjectSpawnManagerList& ObjectSpawnManagersList = ObjectSpawnManagers.FindOrAdd(SpawnObjectType);
		int32 NewSpawnerIndex = ObjectSpawnManagersList.ObjectSpawnManagers.Emplace(NewObject<UObjectSpawnManager>(SpawnManagerOwner, UObjectSpawnManager::StaticClass()));

		CurrentObjectSpawnManager = ObjectSpawnManagersList.ObjectSpawnManagers[NewSpawnerIndex];
	}

	CurrentObjectSpawnManager->Initialize(SpawnObjectType, SpawnParameters, InSpawnManagerOwner, InTag);

	return CurrentObjectSpawnManager;
}

UObjectSpawnManager* UObjectSpawnSubsystem::FindOrAddObjectSpawnManager_Internal(ESpawnObjectType SpawnObjectType, UObject* InSpawnManagerOwner, FName InTag)
{
	if (UObjectSpawnManager* SpawnManager = FindObjectSpawnManager_Internal(SpawnObjectType, InSpawnManagerOwner, InTag))
	{
		return SpawnManager;
	}

	const FObjectSpawnParameters* ObjectTypeSpawnParams = DefaultSpawnParameters.Find(SpawnObjectType);
	return AddObjectSpawnManager_Internal(SpawnObjectType, ObjectTypeSpawnParams == nullptr ? FObjectSpawnParameters{} : *ObjectTypeSpawnParams, InSpawnManagerOwner, InTag);
}

void UObjectSpawnSubsystem::OnObjectSpawned(AActor* SpawnedActor, ESpawnObjectType SpawnObjectType, const UObject* InSpawnManagerOwner, UObjectSpawnManager* SpawnManager)
{
	APawn* AIPawn = Cast<APawn>(SpawnedActor);
	if (IsValid(AIPawn) && SpawnObjectType == ESpawnObjectType::AIPawn)
	{
		AIPawn->SpawnDefaultController();
	}
}

void UObjectSpawnSubsystem::OnSpawnerOwnerChanged(ABaseObjectSpawner* ObjectSpawner, UObject* OldOwner, UObject* NewOwner)
{
	if (!IsValid(ObjectSpawner))
	{
		return;
	}

	UObjectSpawnManager* OldObjectSpawnManager = FindObjectSpawnManager_Internal(ObjectSpawner->GetSpawnObjectType(), OldOwner, ObjectSpawner->GetSpawnTag());
	if (IsValid(OldObjectSpawnManager))
	{
		OldObjectSpawnManager->UnregisterSpawner(ObjectSpawner);
	}

	UObjectSpawnManager* NewObjectSpawnManager = FindOrAddObjectSpawnManager_Internal(ObjectSpawner->GetSpawnObjectType(), NewOwner, ObjectSpawner->GetSpawnTag());
	if (IsValid(NewObjectSpawnManager))
	{
		NewObjectSpawnManager->RegisterSpawner(ObjectSpawner);
	}
}
