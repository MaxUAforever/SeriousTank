#pragma once

#include "Subsystems/ObjectSpawnSubsystem/BaseObjectSpawner.h"
#include "ST_AITeamPawnSpawner.generated.h"

class UST_TeamOwnershipComponent;

UCLASS()
class SERIOUSTANK_API AST_AITeamPawnSpawner : public ABaseObjectSpawner
{
	GENERATED_BODY()

public:
	AST_AITeamPawnSpawner();

	uint8 GetTeamId() const;

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UST_TeamOwnershipComponent> TeamOwnershipComponent;
};
