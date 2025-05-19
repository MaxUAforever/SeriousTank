#pragma once

#include "Subsystems/WorldSubsystem.h"
#include "GenericTeamAgentInterface.h"
#include "ST_AITeamsManagerSubsystem.generated.h"

class ABaseObjectSpawner;
class AST_AITeamPawnSpawner;
class UBaseInteractionAction;
class UInteractionComponent;

struct FTeamInfo
{
	TSet<TObjectPtr<const AActor>> TeamMembers;
};

UCLASS()
class SERIOUSTANK_API UST_AITeamsManagerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	void RegisterTeamSpawner(AST_AITeamPawnSpawner* AITeamPawnSpawner);

private:
	void OnTeamMemberSpawned(ABaseObjectSpawner* ObjectSpawner, AActor* SpawnedActor);
	
	UFUNCTION()
	void OnTeamMemberDestroyed(AActor* DestroyedActor);

private:
	TMap<uint8, FTeamInfo> TeamsInfo;
};
