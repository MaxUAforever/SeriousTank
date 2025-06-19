#pragma once

#include "Subsystems/WorldSubsystem.h"
#include "GenericTeamAgentInterface.h"
#include "ST_AITeamsManagerSubsystem.generated.h"

class ABaseObjectSpawner;
class AST_AITeamPawnSpawner;
class UBaseInteractionAction;
class UInteractionComponent;
class UObjectSpawnSubsystem;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnTeamMemberSpawnedDelegate, uint8, const AActor*);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnTeamMemberDestroyedDelegate, uint8, const AActor*);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTeamDestroyedDelegate, uint8 TeamId);

struct FTeamMemberInfo
{
public:
	FTeamMemberInfo(APawn* InPossessedPawn, AController* InController, AActor* InSpawner)
		: PossessedPawn(InPossessedPawn), Controller(InController), Spawner(InSpawner)
	{
	}

	inline bool operator==(const FTeamMemberInfo& Other) const
	{
		return Spawner == Other.Spawner && PossessedPawn == Other.PossessedPawn && Controller == Other.Controller;
	}

	friend uint32 GetTypeHash(const FTeamMemberInfo& Info);

public:
	TObjectPtr<APawn> PossessedPawn;
	TObjectPtr<AController> Controller;
	TObjectPtr<AActor> Spawner;
};

struct FTeamInfo
{
	TSet<FTeamMemberInfo> TeamMembers;
};

UCLASS()
class SERIOUSTANK_API UST_AITeamsManagerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	FOnTeamMemberSpawnedDelegate OnTeamMemberSpawnedDelegate;
	FOnTeamMemberDestroyedDelegate OnTeamMemberDestroyedDelegate;
	FOnTeamDestroyedDelegate OnTeamDestroyedDelegate;

public:
	void RegisterTeamSpawner(AST_AITeamPawnSpawner* AITeamPawnSpawner);

	const TSet<FTeamMemberInfo>* GetTeamMembers(uint8 TeamId) const;
	int32 GetActiveTeamMembersCount(uint8 TeamId) const;

	void RespawnAllTeamsMembers();

	void AddTeamMember(uint8 TeamId, const FTeamMemberInfo& TeamMemberInfo);

private:
	void OnTeamMemberSpawned(ABaseObjectSpawner* ObjectSpawner, AActor* SpawnedActor);
	void OnTeamMemberDestroyed(AActor* DestroyedActor);

	bool HasAliveTeamMembers(uint8 TeamId) const;
	bool ResetTeamMemberState(FTeamMemberInfo& MemberToReset);

	FTeamInfo& AddTeam(uint8 TeamId);

private:
	TMap<uint8, FTeamInfo> TeamsInfo;

	UObjectSpawnSubsystem* CachedObjectSpawnSubsystem;
};
