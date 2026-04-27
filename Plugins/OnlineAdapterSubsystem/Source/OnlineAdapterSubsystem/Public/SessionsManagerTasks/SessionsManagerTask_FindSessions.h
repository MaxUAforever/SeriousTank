#pragma once

#include "SessionsManagerTasks/BaseSessionsManagerTask.h"

struct FASessionsManagerTask_FindSessions : public FABaseSessionManagerTask
{
public:
	FASessionsManagerTask_FindSessions(TSharedPtr<FASessionManagerState> InSessionManagerState, const FASessionSearchSettings& InSearchSettings, FAOnFindSessionsCompleteDelegate InOnFindSessionsComplete = FAOnFindSessionsCompleteDelegate());
	~FASessionsManagerTask_FindSessions();

protected:
	virtual bool CanBeExecuted() const override;
	virtual bool CanBeAborted() const override;

	virtual void InternalExecute() override;
	virtual void InternalAbort() override;

	virtual void OnExecuteCompleted(bool bWasSuccessful) override;
	virtual void OnAbortCompleted(bool bWasSuccessful) override;

private:
	void OnSessionsFound(bool bWasSuccessful);
	void ClearDelegates();

private:
	FASessionSearchSettings SearchSettings;

	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	FAOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;
	FDelegateHandle FindSessionsDelegateHandle;
	FDelegateHandle CancelFindSessionsDelegateHandle;
};
