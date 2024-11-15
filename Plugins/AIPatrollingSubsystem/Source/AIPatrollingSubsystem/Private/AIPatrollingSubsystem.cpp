#include "AIPatrollingSubsystem.h"

#include "Actors/AIPatrolPath.h"
#include "Components/AIPatrollingComponent.h"

bool UAIPatrollingSubsystem::RegisterPatrolPath(AAIPatrolPath* PatrolPath)
{
	if (IsValid(PatrolPath) && !PatrolPathAgents.Contains(PatrolPath))
	{
		PatrolPathAgents.Add(PatrolPath, {});	
		return true;
	}

	return false;
}

bool UAIPatrollingSubsystem::RegisterAgent(UAIPatrollingComponent* PatrollingComponent, AAIPatrolPath* AssignedPatrolPath)
{
	if (!IsValid(PatrollingComponent) || !IsValid(PatrollingComponent->GetOwner()))
	{
		return false;
	}

	if (IsAgentRegistered(PatrollingComponent))
	{
		return false;
	}

	if (IsValid(AssignedPatrolPath))
	{
		return AssignAgentToPath(PatrollingComponent, AssignedPatrolPath);
	}
	
	FreeAgents.Add(PatrollingComponent->GetOwner());
	return true;
}

bool UAIPatrollingSubsystem::IsAgentRegistered(UAIPatrollingComponent* PatrollingComponent) const
{
	if (!IsValid(PatrollingComponent))
	{
		return false;
	}

	if (FreeAgents.Contains(PatrollingComponent->GetOwner()))
	{
		return true;
	}

	return GetAgentPath(PatrollingComponent) != nullptr;
}

bool UAIPatrollingSubsystem::RemoveAgent(UAIPatrollingComponent* PatrollingComponent)
{
	if (!IsValid(PatrollingComponent))
	{
		return false;
	}

	for (TTuple<AAIPatrolPath*, TArray<AActor*>> PatrolPathAgent : PatrolPathAgents)
	{
		TArray<AActor*>& Agents = PatrolPathAgent.Value;
		if (Agents.Remove(PatrollingComponent->GetOwner()) > 0)
		{
			return true;
		}
	}

	return false;
}

AAIPatrolPath* UAIPatrollingSubsystem::GetAgentPath(UAIPatrollingComponent* PatrollingComponent) const
{
	if (!IsValid(PatrollingComponent))
	{
		return nullptr;
	}

	for (TTuple<AAIPatrolPath*, TArray<AActor*>> PatrolPathAgent : PatrolPathAgents)
	{
		TArray<AActor*>& Agents = PatrolPathAgent.Value;
		if (Agents.Contains(PatrollingComponent->GetOwner()))
		{
			return PatrolPathAgent.Key;
		}
	}

	return nullptr;
}

FPatrolPathPointInfo UAIPatrollingSubsystem::FindNearestPatrolPoint(const FVector& Location, bool bCheckPriority, bool bCheckAvailable)
{
	float MinDistance = TNumericLimits<float>::Max();
	int32 CurrentPriority = 0;

	FPatrolPathPointInfo Result;
	for (TTuple<AAIPatrolPath*, TArray<AActor*>> PatrolPathAgent : PatrolPathAgents)
	{
		AAIPatrolPath* PatrolPath = PatrolPathAgent.Key;
		const TArray<AActor*>& Agents = PatrolPathAgent.Value;
		if (!IsValid(PatrolPath))
		{
			continue;
		}

		const bool bIsNotAvaliable = bCheckAvailable && Agents.Num() >= PatrolPath->GetMaxPatrollingAgents();
		const bool bLowPriority = bCheckPriority && PatrolPath->GetPriority() < CurrentPriority;
		if (bIsNotAvaliable || bLowPriority)
		{
			continue;
		}

		int32 Index = FindNearestPointOfPath(Location, PatrolPath);
		if (Index == -1)
		{
			continue;
		}
		
		const float CurrentDistance = FVector::DistSquared(Location, PatrolPath->GetPathPoints()[Index]);
		if (CurrentDistance < MinDistance)
		{
			MinDistance = CurrentDistance;

			Result.PatrolPath = PatrolPath;
			Result.Index = Index;
			CurrentPriority = PatrolPath->GetPriority();
		}
	}

	return Result;
}

int32 UAIPatrollingSubsystem::FindNearestPointOfPath(const FVector& Location, AAIPatrolPath* PatrolPath)
{
	int32 PointIndex = -1;
	float MinDistance = TNumericLimits<float>::Max();

	int32 CurrentIndex = 0;
	for (const FVector& PatrolPoint : PatrolPath->GetPathPoints())
	{
		const float CurrentDistance = FVector::DistSquared(Location, PatrolPoint);
		if (CurrentDistance < MinDistance)
		{
			MinDistance = CurrentDistance;

			PointIndex = CurrentIndex;
		}

		CurrentIndex += 1;
	}

	return PointIndex;
}

bool UAIPatrollingSubsystem::AssignAgentToPath(UAIPatrollingComponent* PatrollingComponent, AAIPatrolPath* AssignedPatrolPath)
{
	if (!IsValid(PatrollingComponent) || !IsValid(AssignedPatrolPath))
	{
		return false;
	}

	if (PatrolPathAgents.Find(AssignedPatrolPath)  == nullptr)
	{
		PatrolPathAgents.Add(AssignedPatrolPath, {});
	}

	if (PatrolPathAgents[AssignedPatrolPath].Num() >= AssignedPatrolPath->GetMaxPatrollingAgents())
	{
		return false;
	}
	
	PatrolPathAgents[AssignedPatrolPath].AddUnique(PatrollingComponent->GetOwner());
	if (AActor* Agent = PatrollingComponent->GetOwner())
	{
		PatrollingComponent->SetPatrolPath(AssignedPatrolPath);
		PatrollingComponent->SetCurrentDirection(AssignedPatrolPath->GetInitialDirectionType());
		PatrollingComponent->SetCurrentPathPointIndex(FindNearestPointOfPath(Agent->GetActorLocation(), AssignedPatrolPath));
	}

	return true;
}

bool UAIPatrollingSubsystem::StartPatrolling(UAIPatrollingComponent* PatrollingComponent)
{
	AActor* Agent = PatrollingComponent->GetOwner();
	if (!IsValid(PatrollingComponent) || !IsValid(Agent))
	{
		return false;
	}

	bool bPathIsFound = IsValid(GetAgentPath(PatrollingComponent));
	if (!bPathIsFound)
	{
		FPatrolPathPointInfo PatrolPathInfo = FindNearestPatrolPoint(Agent->GetActorLocation(), true, true);
		bPathIsFound = AssignAgentToPath(PatrollingComponent, PatrolPathInfo.PatrolPath);
	}

	if (bPathIsFound)
	{
		PatrollingComponent->SetIsPatrollingActive(true);
	}

	return bPathIsFound;
}
