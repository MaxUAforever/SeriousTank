#pragma once

#include "GameFramework/Gameplay/Quests/ST_QuestsCompletingGameMode.h"
#include "QuestSubsystem/Public/QuestCoreTypes.h"
#include "ST_AITeamMatchGameMode.generated.h"

UCLASS()
class SERIOUSTANK_API AST_AITeamMatchGameMode : public AST_QuestsCompletingGameMode
{
	GENERATED_BODY()

protected:
	virtual void OnQuestsStarted() override;

	virtual void OnPostLogin(AController* NewPlayer) override;

private:
	void OnRoundStarted(int32 RoundNumber);
};
