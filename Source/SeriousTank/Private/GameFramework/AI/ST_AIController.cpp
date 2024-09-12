#include "GameFramework/AI/ST_AIController.h"

void AST_AIController::BeginPlay()
{
	Super::BeginPlay();

	if (DefaultBehaviourTree)
	{
		RunBehaviorTree(DefaultBehaviourTree);
	}
}