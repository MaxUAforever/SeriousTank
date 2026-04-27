#pragma once

#include "SessionsManagerTasks/BaseSessionsManagerTask.h"

struct FASessionsManagerTask_DestroySession : public FABaseSessionManagerTask
{
public:
	FASessionsManagerTask_DestroySession(TSharedPtr<FASessionManagerState> InSessionManagerState, FName InSessionName, FAOnDestroySessionCompleteDelegate InOnDestroySessionComplete = FAOnDestroySessionCompleteDelegate());
	~FASessionsManagerTask_DestroySession();

protected:
	virtual bool CanBeExecuted() const override;
	virtual void InternalExecute() override;
	virtual void OnExecuteCompleted(bool bWasSuccessful) override;

private:
	void OnSessionDestroyed(FName SessionName, bool bWasSuccessful);

	void ClearDelegates();

private:
	FName SessionName;

	FAOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;
	FDelegateHandle DestroySessionDelegateHandle;
};
