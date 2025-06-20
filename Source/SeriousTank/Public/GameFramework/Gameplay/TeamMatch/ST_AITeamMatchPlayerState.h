#pragma once

#include "GameFramework/Gameplay/ST_GameplayPlayerState.h"
#include "ST_AITeamMatchPlayerState.generated.h"

UCLASS()
class SERIOUSTANK_API AST_AITeamMatchPlayerState : public AST_GameplayPlayerState
{
	GENERATED_BODY()

public:
	inline int32 GetKillNum() const { return KillNum; }
	inline void AddKillNum(int32 Num) { KillNum += Num; }

	inline int32 GetDeathNum() const { return DeathNum; }
	inline void AddDeathNum(int32 Num) { DeathNum += Num; }

private:
	int32 KillNum = 0;
	int32 DeathNum = 0;
};
