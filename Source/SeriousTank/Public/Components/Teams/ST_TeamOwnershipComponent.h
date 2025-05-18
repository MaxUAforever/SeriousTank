#pragma once

#include "Components/ActorComponent.h"
#include "GenericTeamAgentInterface.h"
#include "ST_TeamOwnershipComponent.generated.h"

class UST_TeamOwnershipComponent;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnTeamOwnershipChanged, const UST_TeamOwnershipComponent*, const FGenericTeamId&);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SERIOUSTANK_API UST_TeamOwnershipComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	FOnTeamOwnershipChanged OnTeamOwnershipChangedDelegate;

public:
	UST_TeamOwnershipComponent();

	void SetGenericTeamId(const FGenericTeamId& NewTeamID);
	const FGenericTeamId& GetGenericTeamId() const { return TeamID; }

protected:
	UPROPERTY(EditAnywhere)
	FGenericTeamId TeamID;
};
