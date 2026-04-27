#pragma once

#include "SessionsManagerTasks/BaseSessionsManagerTask.h"
#include "Interfaces/OnlineSessionInterface.h"

struct FASessionsManagerTask_JoinSession : public FABaseSessionManagerTask
{
public:
	FASessionsManagerTask_JoinSession(TSharedPtr<FASessionManagerState> InSessionManagerState, FName InSessionName, const FOnlineSessionSearchResult& InDesiredSession, FAOnJoinSessionCompleteDelegate InOnJoinSessionComplete);
	~FASessionsManagerTask_JoinSession();

protected:
	virtual bool CanBeExecuted() const override;
	virtual void InternalExecute() override;
	virtual void OnExecuteCompleted(bool bWasSuccessful) override;

private:
	void OnSessionJoined(FName InSessionName, EOnJoinSessionCompleteResult::Type JoinResult);

	void ClearDelegates();

private:
	FName SessionName;
	FOnlineSessionSearchResult DesiredSession;
		
	FAOnJoinSessionCompleteDelegate OnJoinSessionComplete;
	FDelegateHandle JoinSessionDelegateHandle;
};
