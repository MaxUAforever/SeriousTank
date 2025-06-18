#include "Subsystems/AIManagerSubsystem/ST_AITeamsManagerSubsystem.h"

#include "GameFramework/AI/ST_AIController.h"
#include "Components/Teams/ST_TeamOwnershipComponent.h"
#include "GameFramework/Pawn.h"
#include "Subsystems/AIManagerSubsystem/Actors/ST_AITeamPawnSpawner.h"
#include "Subsystems/HealthSubsystem/ST_HealthComponent.h"
#include "Subsystems/ObjectSpawnSubsystem/ObjectSpawnSubsystem.h"
#include "Engine/World.h"

uint32 GetTypeHash(const FTeamMemberInfo& Info)
{
	return HashCombine(GetTypeHash(Info.Controller), GetTypeHash(Info.PossessedPawn));
}

void UST_AITeamsManagerSubsystem::RegisterTeamSpawner(AST_AITeamPawnSpawner* AITeamPawnSpawner)
{
	if (!IsValid(AITeamPawnSpawner))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s::%s: Invalid AITeamPawnSpawner provided"), *GetClass()->GetName(), TEXT("RegisterTeamSpawner"));
		return;
	}

	const UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s::%s: Invalid World context"), *GetClass()->GetName(), TEXT("RegisterTeamSpawner"));
		return;
	}

	CachedObjectSpawnSubsystem = World->GetSubsystem<UObjectSpawnSubsystem>();
	if (!IsValid(CachedObjectSpawnSubsystem))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s::%s: Invalid ObjectSpawnSubsystem context"), *GetClass()->GetName(), TEXT("RegisterTeamSpawner"));
		return;
	}

	const FName TeamTag = (AITeamPawnSpawner->GetSpawnTag() == NAME_None)
		? FName(*FString::Printf(TEXT("Team_%d"), AITeamPawnSpawner->GetTeamId()))
		: AITeamPawnSpawner->GetSpawnTag();

	AITeamPawnSpawner->SetSpawnTag(TeamTag);
	AITeamPawnSpawner->SetSpawnOwner(this);

	if (!TeamsInfo.Contains(AITeamPawnSpawner->GetTeamId()))
	{
		TeamsInfo.Add(AITeamPawnSpawner->GetTeamId());

		FOnObjectSpawnedDelegate* OnObjectSpawnedDelegate = CachedObjectSpawnSubsystem->GetManagerObjectSpawnedDelegate(ESpawnObjectType::AIPawn, this, TeamTag);
		if (OnObjectSpawnedDelegate != nullptr)
		{
			OnObjectSpawnedDelegate->AddUObject(this, &ThisClass::OnTeamMemberSpawned);
		}
	}
}

const TSet<FTeamMemberInfo>* UST_AITeamsManagerSubsystem::GetTeamMembers(uint8 TeamId) const
{
	const FTeamInfo* TeamInfo = TeamsInfo.Find(TeamId);
	if (TeamInfo == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s::%s: Team with ID %d not found"), *GetClass()->GetName(), TEXT("GetTeamMembers"), TeamId);
		return nullptr;
	}

	return &TeamInfo->TeamMembers;
}

int32 UST_AITeamsManagerSubsystem::GetActiveTeamMembersCount(uint8 TeamId) const
{
	const TSet<FTeamMemberInfo>* TeamMembers = GetTeamMembers(TeamId);
	if (TeamMembers == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s::%s: Team with ID %d not found"), *GetClass()->GetName(), TEXT("GetActiveTeamMembersCount"), TeamId);
		return -1;
	}

	return TeamMembers->Num();
}

void UST_AITeamsManagerSubsystem::RespawnAllTeamsMembers()
{
	if (!IsValid(CachedObjectSpawnSubsystem))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s::%s: CachedObjectSpawnSubsystem is not valid"), *GetClass()->GetName(), TEXT("RespawnAllTeamsMembers"));
		return;
	}

	for (const auto& [TeamId, TeamInfo] : TeamsInfo)
	{
		for (const FTeamMemberInfo& Member : TeamInfo.TeamMembers)
		{
			const bool bResetSuccessfully = ResetTeamMemberState(Member);
			
			if (!bResetSuccessfully)
			{
				UE_LOG(LogTemp, Warning, TEXT("%s::%s: Failed to reset state for team member %s"), *GetClass()->GetName(), TEXT("RespawnAllTeamsMembers"), *Member.PossessedPawn->GetName());
			}
		}

		const FName TeamTag = FName(*FString::Printf(TEXT("Team_%d"), TeamId));
		const bool bIsSpawnSuccessfull = CachedObjectSpawnSubsystem->SpawnInAllAvailableSpawners(ESpawnObjectType::AIPawn, this, TeamTag);
		
		if (!bIsSpawnSuccessfull)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s::%s: Failed to respawn team members for team ID %d"), *GetClass()->GetName(), TEXT("RespawnAllTeamsMembers"), TeamId);
		}
	}
}

void UST_AITeamsManagerSubsystem::OnTeamMemberSpawned(ABaseObjectSpawner* ObjectSpawner, AActor* SpawnedActor)
{
	APawn* SpawnedPawn = Cast<APawn>(SpawnedActor);
	if (!IsValid(ObjectSpawner) || !IsValid(SpawnedPawn))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s::%s: Invalid ObjectSpawner or SpawnedPawn"), *GetClass()->GetName(), TEXT("OnTeamMemberSpawned"));
		return;
	}

	AST_AIController* AIController = Cast<AST_AIController>(SpawnedPawn->GetController());
	if (!IsValid(AIController))
	{
		SpawnedPawn->SpawnDefaultController();

		AIController = Cast<AST_AIController>(SpawnedPawn->GetController());
		if (!IsValid(AIController))
		{
			UE_LOG(LogTemp, Warning, TEXT("%s::%s: SpawnedPawn does not have a valid AIController"), *GetClass()->GetName(), TEXT("OnTeamMemberSpawned"));
			return;
		}

		AIController->SetEnemyType(EEnemyType::Team);
	}

	UST_HealthComponent* HealthComponent = SpawnedPawn->GetComponentByClass<UST_HealthComponent>();
	if (!IsValid(HealthComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s::%s: SpawnedPawn does not have a valid HealthComponent"), *GetClass()->GetName(), TEXT("OnTeamMemberSpawned"));
		return;
	}

	const UST_TeamOwnershipComponent* TeamOwnershipComponent = ObjectSpawner->GetComponentByClass<UST_TeamOwnershipComponent>();
	if (!IsValid(TeamOwnershipComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s::%s: SpawnedPawn does not have a valid TeamOwnershipComponent"), *GetClass()->GetName(), TEXT("OnTeamMemberSpawned"));
		return;
	}

	const FGenericTeamId& TeamID = TeamOwnershipComponent->GetGenericTeamId();
	AIController->SetGenericTeamId(TeamID);
	HealthComponent->OnOwnerKilledDelegate.AddUObject(this, &ThisClass::OnTeamMemberDestroyed);

	FTeamInfo& TeamInfo = TeamsInfo.FindOrAdd(TeamID.GetId());
	TeamInfo.TeamMembers.Add({ SpawnedPawn, AIController, ObjectSpawner });

	OnTeamMemberSpawnedDelegate.Broadcast(TeamID.GetId(), SpawnedActor);
}

void UST_AITeamsManagerSubsystem::OnTeamMemberDestroyed(AActor* DestroyedActor)
{
	const APawn* DestroyedPawn = Cast<APawn>(DestroyedActor);
	if (!IsValid(DestroyedPawn))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s::%s: Invalid SpawnedPawn"), *GetClass()->GetName(), TEXT("OnTeamMemberDestroyed"));
		return;
	}

	AAIController* AIController = Cast<AAIController>(DestroyedPawn->GetController());
	if (!IsValid(AIController))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s::%s: SpawnedPawn does not have a valid AIController"), *GetClass()->GetName(), TEXT("OnTeamMemberDestroyed"));
		return;
	}

	OnTeamMemberDestroyedDelegate.Broadcast(AIController->GetGenericTeamId().GetId(), DestroyedActor);

	if (!HasAliveTeamMembers(AIController->GetGenericTeamId().GetId()))
	{
		OnTeamDestroyedDelegate.Broadcast(AIController->GetGenericTeamId().GetId());
	}
}

bool UST_AITeamsManagerSubsystem::HasAliveTeamMembers(uint8 TeamId) const
{
	const TSet<FTeamMemberInfo>* TeamMembers = GetTeamMembers(TeamId);
	if (TeamMembers == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s::%s: Team with ID %d not found"), *GetClass()->GetName(), TEXT("HasAliveTeamMembers"), TeamId);
		return false;
	}

	for (const FTeamMemberInfo& Member : *TeamMembers)
	{
		if (!IsValid(Member.PossessedPawn))
		{
			UE_LOG(LogTemp, Warning, TEXT("%s::%s: Team member %s is not valid"), *GetClass()->GetName(), TEXT("HasAliveTeamMembers"), *Member.PossessedPawn->GetName());
			continue;
		}

		const UST_HealthComponent* HealthComponent = Member.PossessedPawn->GetComponentByClass<UST_HealthComponent>();
		if (!IsValid(HealthComponent))
		{
			UE_LOG(LogTemp, Warning, TEXT("%s::%s: Team member %s does not have a valid HealthComponent"), *GetClass()->GetName(), TEXT("HasAliveTeamMembers"), *Member.PossessedPawn->GetName());
			continue;
		}

		if (HealthComponent->GetCurrentHealth() > 0.f)
		{
			return true;
		}
	}

	return false;
}

bool UST_AITeamsManagerSubsystem::ResetTeamMemberState(const FTeamMemberInfo& MemberToReset)
{
	if (!IsValid(MemberToReset.PossessedPawn) || !IsValid(MemberToReset.Controller))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s::%s: Invalid TeamMemberInfo provided"), *GetClass()->GetName(), TEXT("ResetTeamMemberState"));
		return false;
	}

	UST_HealthComponent* HealthComponent = MemberToReset.PossessedPawn->GetComponentByClass<UST_HealthComponent>();
	if (!IsValid(HealthComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s::%s: Team member %s does not have a valid HealthComponent"), *GetClass()->GetName(), TEXT("ResetTeamMemberState"), *MemberToReset.PossessedPawn->GetName());
		return false;
	}

	if (HealthComponent->GetCurrentHealth() > 0.f)
	{
		HealthComponent->AddHealthValue(HealthComponent->GetMaxHealth());
	}

	MemberToReset.PossessedPawn->SetActorLocation(MemberToReset.Spawner->GetActorLocation());
	return true;
}
