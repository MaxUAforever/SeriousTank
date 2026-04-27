#pragma once

#include "OnlineAdapterTypes.h"
#include "OnlineSessionSettings.h"

DECLARE_DELEGATE_OneParam(FAOnSessionManagerTaskCompleteDelegate, bool /*bWasSuccessful*/);
DECLARE_DELEGATE_OneParam(FAOnSessionManagerTaskAbortedDelegate, bool /*bWasSuccessful*/);

class FABaseSessionManagerTask : public TSharedFromThis<FABaseSessionManagerTask>
{
public:
	FAOnSessionManagerTaskCompleteDelegate OnExecuteCompletedDelegate;
	FAOnSessionManagerTaskAbortedDelegate OnTaskAbortedDelegate;

	virtual ~FABaseSessionManagerTask() = default;

public:
	FABaseSessionManagerTask(TSharedPtr<FASessionManagerState> InSessionManagerState);

	void Execute();
	void Abort();

protected:
	TSharedPtr<FASessionManagerState> GetSessionManagerState() const { return CurrentSessionManagerState.Pin(); }

	const FNamedOnlineSession* GetSession(FName SessionName) const;
	bool IsInSession(FName SessionName) const;
	
protected:
	virtual bool CanBeExecuted() const { return true; }
	virtual bool CanBeAborted() const { return false; }

	virtual void InternalExecute() = 0;
	virtual void InternalAbort() {};

	virtual void OnExecuteCompleted(bool bWasSuccessful);
	virtual void OnAbortCompleted(bool bWasSuccessful);

private:
	TWeakPtr<FASessionManagerState> CurrentSessionManagerState;
};
