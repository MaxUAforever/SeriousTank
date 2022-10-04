#pragma once

#include "GameFramework/GameModeBase.h"
#include "ST_GameplayGameMode.generated.h"


UCLASS()
class SERIOUSTANK_API AST_GameplayGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
};
