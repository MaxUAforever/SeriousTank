#pragma once

#include "GameFramework/PlayerStart.h"
#include "ST_TeamPlayerStart.generated.h"

class UST_TeamOwnershipComponent;

UCLASS()
class SERIOUSTANK_API AST_TeamPlayerStart : public APlayerStart
{
	GENERATED_BODY()

public:
	AST_TeamPlayerStart(const FObjectInitializer& ObjectInitializer);

	uint8 GetTeamId() const;

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UST_TeamOwnershipComponent> TeamOwnershipComponent;
};
