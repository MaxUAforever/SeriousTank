#pragma once

#include "Subsystems/WorldSubsystem.h"
#include "AIPatrollingSubsystem.generated.h"

class AAIPatrolPath;
class UAIPatrollingComponent;

struct FPatrolPathPointInfo
{
	AAIPatrolPath* PatrolPath = nullptr;
	int32 Index = 0;
};


/**
 * Subsystem that allows to assign AI agents to patrol the route according to specified rules 
 * and monitor the current status of routes and agents
 */
UCLASS()
class AIPATROLLINGSUBSYSTEM_API UAIPatrollingSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	bool RegisterPatrolPath(AAIPatrolPath* PatrolPath);
	bool RegisterAgent(UAIPatrollingComponent* PatrollingComponent, AAIPatrolPath* AssignedPatrolPath = nullptr);

	bool IsAgentRegistered(UAIPatrollingComponent* PatrollingComponent) const;
	bool RemoveAgent(UAIPatrollingComponent* PatrollingComponent);

	AAIPatrolPath* GetAgentPath(UAIPatrollingComponent* PatrollingComponent) const;

	/**
	 * Allows to find the closest registered patrol point for location.
	 * 
	 * @param Location - the position relative to which one the nearest patrol point should be found.
	 * @param bCheckPriority - flag which determines if point with the highest priority should be found.
	 * @param bCheckAvailable - flag which determines if path with max number of agents should be ignored.
	 * @return FPatrolPathPointInfo - information about closest patrol point
	 */
	FPatrolPathPointInfo FindNearestPatrolPoint(const FVector& Location, bool bCheckPriority, bool bCheckAvailable);

	/**
	 * Allows to find the closest registered point of patrol path for location.
	 *
	 * @param Location - the position relative to which one the nearest patrol point should be found.
	 * @param PatrolPath - Patrol path actor that is used for searching .
	 * @return int32 - index of founded point. -1 is return in case of wrong params
	 */
	int32 FindNearestPointOfPath(const FVector& Location, AAIPatrolPath* PatrolPath);
	
	/**
	 * Allows to set needed patrol path for specific agent for handling.
	 */
	bool AssignAgentToPath(UAIPatrollingComponent* PatrollingComponent, AAIPatrolPath* AssignedPatrolPath);
	
	/**
	 * Allows to initiate patrolling for specefic agent. 
	 * If path is not defined - subsystem will try to find the nearest path with highest priority.
	 */
	bool StartPatrolling(UAIPatrollingComponent* PatrollingComponent);

private:
	TMap<AAIPatrolPath*, TArray<AActor*>> PatrolPathAgents;
	TSet<AActor*> FreeAgents;
};
