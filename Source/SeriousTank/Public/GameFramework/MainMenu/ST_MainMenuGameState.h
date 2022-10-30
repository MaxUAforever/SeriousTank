#pragma once

#include "GameFramework/GameStateBase.h"
#include "ST_MainMenuGameState.generated.h"

UCLASS()
class SERIOUSTANK_API AST_MainMenuGameState : public AGameStateBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	int32 CurrentGameLevelIndex;

public:
	AST_MainMenuGameState();

	int32 GetCurrentGameLevelIndex() const
	{
		return CurrentGameLevelIndex;
	}

	void SetCurrentGameLevelIndex(int32 Index);
};
