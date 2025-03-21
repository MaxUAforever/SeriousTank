#pragma once

#include "GameFramework/Gameplay/ST_GameplayGameMode.h"
#include "QuestSubsystem/Public/QuestCoreTypes.h"
#include "ST_QuestsCompletingGameMode.generated.h"

class UQuestProvider;
enum class EQuestTaskCompleteResult : uint8;

UCLASS()
class SERIOUSTANK_API AST_QuestsCompletingGameMode : public AST_GameplayGameMode
{
	GENERATED_BODY()
	
public:
	AST_QuestsCompletingGameMode();

protected:
	virtual void OnPreStartCountdownEneded() override;

private:
	void StartQuests();
	
	void OnQuestTaskCompleted(FTaskID TaskID, EQuestTaskCompleteResult TaskCompleteResult);

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UQuestProvider> QuestProvider;
};
