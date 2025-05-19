#include "Subsystems/AIManagerSubsystem/ST_AITeamsManagerSubsystem.h"

#include "AIController.h"
#include "Components/Teams/ST_TeamOwnershipComponent.h"
#include "GameFramework/Pawn.h"
#include "Subsystems/AIManagerSubsystem/Actors/ST_AITeamPawnSpawner.h"
#include "Subsystems/ObjectSpawnSubsystem/ObjectSpawnSubsystem.h"
#include "Engine/World.h"

void UST_AITeamsManagerSubsystem::RegisterTeamSpawner(AST_AITeamPawnSpawner* AITeamPawnSpawner)
{
	if (!IsValid(AITeamPawnSpawner))
	{
		return;
	}

	const UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return;
	}

	UObjectSpawnSubsystem* ObjectSpawnSubsystem = World->GetSubsystem<UObjectSpawnSubsystem>();
	if (!IsValid(ObjectSpawnSubsystem))
	{
		return;
	}

	ObjectSpawnSubsystem->FindOrAddObjectSpawnManager(ESpawnObjectType::AIPawn, this);
	FOnObjectSpawnedDelegate* OnObjectSpawnedDelegate = ObjectSpawnSubsystem->GetManagerObjectSpawnedDelegate(ESpawnObjectType::AIPawn, this);
	if (OnObjectSpawnedDelegate != nullptr)
	{
		OnObjectSpawnedDelegate->AddUObject(this, &ThisClass::OnTeamMemberSpawned);
	}

	// TODO: setup spawn parameters

	AITeamPawnSpawner->SetSpawnOwner(this);
}

void UST_AITeamsManagerSubsystem::OnTeamMemberSpawned(ABaseObjectSpawner* ObjectSpawner, AActor* SpawnedActor)
{
	const APawn* SpawnedPawn = Cast<APawn>(SpawnedActor);
	if (!IsValid(ObjectSpawner) || !IsValid(SpawnedPawn))
	{
		return;
	}

	AAIController* AIController = Cast<AAIController>(SpawnedPawn->GetController());
	if (!IsValid(AIController))
	{
		return;
	}

	const UST_TeamOwnershipComponent* TeamOwnershipComponent = ObjectSpawner->GetComponentByClass<UST_TeamOwnershipComponent>();
	if (!IsValid(TeamOwnershipComponent))
	{
		return;
	}

	const FGenericTeamId& TeamID = TeamOwnershipComponent->GetGenericTeamId();
	AIController->SetGenericTeamId(TeamID);
	AIController->OnDestroyed.AddDynamic(this, &ThisClass::OnTeamMemberDestroyed);

	FTeamInfo& TeamInfo = TeamsInfo.FindOrAdd(TeamID.GetId());
	TeamInfo.TeamMembers.Add(AIController);
}

void UST_AITeamsManagerSubsystem::OnTeamMemberDestroyed(AActor* DestroyedActor)
{
	for (auto& [TeamId, TeamInfo] : TeamsInfo)
	{
		if (TeamInfo.TeamMembers.Contains(DestroyedActor))
		{
			TeamInfo.TeamMembers.Remove(DestroyedActor);
			break;
		}
	}
}
