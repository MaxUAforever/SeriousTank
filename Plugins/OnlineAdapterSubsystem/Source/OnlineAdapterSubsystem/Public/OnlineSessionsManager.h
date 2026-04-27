#pragma once

#include "OnlineBaseManager.h"
#include "OnlineAdapterTypes.h"
#include "Delegates/IDelegateInstance.h"
#include "SessionsManagerTasks/BaseSessionsManagerTask.h"

class FOnlineSessionsManager : public FOnlineBaseManager, public TSharedFromThis<FOnlineSessionsManager>
{
public:
	static TSharedPtr<FOnlineSessionsManager> Create();

public:
	FOnlineSessionsManager();
	~FOnlineSessionsManager();

	void CreateSession(FName SessionName, const FOnlineSessionSettings& NewSessionSettings, FAOnCreateSessionCompleteDelegate OnCreateSessionComplete);
	void DestroySession(FName InSessionName, FAOnDestroySessionCompleteDelegate OnDestroySessionComplete = FAOnDestroySessionCompleteDelegate());
	void FindSessions(const FASessionSearchSettings& SessionSearchSettings);
	void JoinSession(FName SessionName, const FOnlineSessionSearchResult& DesiredSession, FAOnJoinSessionCompleteDelegate OnJoinSessionComplete);

	void StopCurrentTask();
	void StopAllTasks();

private:
	void AddNewTask(TSharedPtr<FABaseSessionManagerTask> NewTask);
	void ProcessNextTask();

	void OnCurrentTaskCompleted(bool bWasSuccessful);
	void OnCurrentTaskAborted(bool bWasSuccessful);

private:
	TSharedPtr<FASessionManagerState> SessionManagerState;

	TSharedPtr<FABaseSessionManagerTask> CurrentTask;
	TArray<TSharedPtr<FABaseSessionManagerTask>> TasksQueue;

	bool bIsShuttingDown = false;
};
