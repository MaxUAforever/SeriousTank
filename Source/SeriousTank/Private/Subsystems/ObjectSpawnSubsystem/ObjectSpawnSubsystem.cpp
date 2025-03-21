#include "Subsystems/ObjectSpawnSubsystem/ObjectSpawnSubsystem.h"

#include "Engine/EngineBaseTypes.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "Subsystems/ObjectSpawnSubsystem/ObjectSpawnManager.h"

uint32 GetTypeHash(const FObjectSpawnManagerInfo& ObjectSpawnManagerInfo)
{
	return HashCombine(GetTypeHash(ObjectSpawnManagerInfo.SpawnManagerOwner), GetTypeHash(ObjectSpawnManagerInfo.SpawnObjectType));
}

bool UObjectSpawnSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	UWorld* World = Cast<UWorld>(Outer);

	return Super::ShouldCreateSubsystem(Outer) && World && !World->IsNetMode(NM_Client);
}

const UObjectSpawnManager* UObjectSpawnSubsystem::AddObjectSpawnManager(ESpawnObjectType SpawnObjectType, const FObjectSpawnParameters& SpawnParameters, UObject* InSpawnManagerOwner)
{
	UObject* SpawnManagerOwner = InSpawnManagerOwner ? InSpawnManagerOwner : GetWorld();
	if (!IsValid(SpawnManagerOwner))
	{
		UE_LOG(LogTemp, Warning, TEXT("ObjectSpawnSubsystem::AddObjectSpawnManager: Trying to add SpawnManager with invalid Owner"));
		return nullptr;
	}

	TObjectPtr<UObjectSpawnManager> CurrentObjectSpawnManager = FindObjectSpawnManager(SpawnObjectType, SpawnManagerOwner);
	if (!IsValid(CurrentObjectSpawnManager))
	{
		CurrentObjectSpawnManager = ObjectSpawnManagers.Emplace({ SpawnObjectType, SpawnManagerOwner }, NewObject<UObjectSpawnManager>(SpawnManagerOwner));
	}

	CurrentObjectSpawnManager->Initialize(SpawnObjectType, SpawnParameters, InSpawnManagerOwner);

	return CurrentObjectSpawnManager;
}

const UObjectSpawnManager* UObjectSpawnSubsystem::GetObjectSpawnManager(ESpawnObjectType SpawnObjectType, const UObject* InSpawnManagerOwner)
{
	return FindObjectSpawnManager(SpawnObjectType, InSpawnManagerOwner);
}

const UObjectSpawnManager* UObjectSpawnSubsystem::FindOrAddObjectSpawnManager(ESpawnObjectType SpawnObjectType, UObject* InSpawnManagerOwner)
{
	const UObject* SpawnManagerOwner = InSpawnManagerOwner ? InSpawnManagerOwner : GetWorld();
	if (!IsValid(SpawnManagerOwner))
	{
		UE_LOG(LogTemp, Warning, TEXT("ObjectSpawnSubsystem::FindOrAddObjectSpawnManager: Trying to get SpawnManager with invalid Owner"));
		return nullptr;
	}

	if (const UObjectSpawnManager* SpawnManager = FindObjectSpawnManager(SpawnObjectType, SpawnManagerOwner))
	{
		return SpawnManager;
	}
	
	return AddObjectSpawnManager(SpawnObjectType, FObjectSpawnParameters(), InSpawnManagerOwner);
}

void UObjectSpawnSubsystem::SetupSpawnManager(ESpawnObjectType SpawnObjectType, const FObjectSpawnParameters& SpawnParameters, const UObject* InSpawnManagerOwner)
{
	UObjectSpawnManager* SpawnManager = FindObjectSpawnManager(SpawnObjectType, InSpawnManagerOwner);
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

AActor* UObjectSpawnSubsystem::SpawnObject(ESpawnObjectType SpawnObjectType, const UObject* InSpawnManagerOwner)
{
	UObjectSpawnManager* SpawnManager = FindObjectSpawnManager(SpawnObjectType, InSpawnManagerOwner);
	if (!IsValid(SpawnManager))
	{
		UE_LOG(LogTemp, Warning, TEXT("ObjectSpawnSubsystem::SpawnObject: Trying to spawn object with not existing SpawnManager"));
		return nullptr;
	}

	AActor* SpawnedActor = SpawnManager->SpawnRandomObject();
	OnObjectSpawned(SpawnedActor, SpawnObjectType, InSpawnManagerOwner, SpawnManager);

	return SpawnedActor;
}

FOnObjectSpawnedDelegate* UObjectSpawnSubsystem::GetManagerObjectSpawnedDelegate(ESpawnObjectType SpawnObjectType, const UObject* InSpawnManagerOwner)
{
	UObjectSpawnManager* SpawnManager = FindObjectSpawnManager(SpawnObjectType, InSpawnManagerOwner);
	if (!IsValid(SpawnManager))
	{
		UE_LOG(LogTemp, Warning, TEXT("ObjectSpawnSubsystem::GetManagerObjectSpawnedDelegate: Trying to get delegate with not existing SpawnManager"));
		return nullptr;
	}

	return &SpawnManager->OnObjectSpawnedDelegate;
}

UObjectSpawnManager* UObjectSpawnSubsystem::FindObjectSpawnManager(ESpawnObjectType SpawnObjectType, const UObject* InSpawnManagerOwner = nullptr)
{
	const UObject* SpawnManagerOwner = InSpawnManagerOwner ? InSpawnManagerOwner : GetWorld();
	if (!IsValid(SpawnManagerOwner))
	{
		UE_LOG(LogTemp, Warning, TEXT("ObjectSpawnSubsystem::FindObjectSpawnManager: Trying to get SpawnManager with invalid Owner"));
		return nullptr;
	}

	TObjectPtr<UObjectSpawnManager>* SpawnManagerPtr = ObjectSpawnManagers.Find({ SpawnObjectType, SpawnManagerOwner });

	return SpawnManagerPtr ? *SpawnManagerPtr : nullptr;
}

void UObjectSpawnSubsystem::OnObjectSpawned(AActor* SpawnedActor, ESpawnObjectType SpawnObjectType, const UObject* InSpawnManagerOwner, UObjectSpawnManager* SpawnManager)
{
	APawn* AIPawn = Cast<APawn>(SpawnedActor);
	if (IsValid(AIPawn) && SpawnObjectType == ESpawnObjectType::AIPawn)
	{
		AIPawn->SpawnDefaultController();
	}
}
