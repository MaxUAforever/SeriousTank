#pragma once

#include "SessionsManagerTasks/BaseSessionsManagerTask.h"

struct FASessionsManagerTask_CreateSession : public FABaseSessionManagerTask
{
public:
	FASessionsManagerTask_CreateSession(TSharedPtr<FASessionManagerState> InSessionManagerState, FName InSessionName, const FOnlineSessionSettings& InSessionSettings, FAOnCreateSessionCompleteDelegate InOnCreateSessionComplete);
	~FASessionsManagerTask_CreateSession();

protected:
	virtual bool CanBeExecuted() const override;
	virtual void InternalExecute() override;
	virtual void OnExecuteCompleted(bool bWasSuccessful) override;

private:
	void OnSessionCreated(FName InSessionName, bool bWasSuccessful);

	void ClearDelegates();

private:
	FName SessionName;
	FOnlineSessionSettings SessionSettings;

	FAOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	FDelegateHandle CreateSessionDelegateHandle;
};
