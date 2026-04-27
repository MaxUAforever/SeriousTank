#include "OnlineSessionsManager.h"

#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystem.h"
#include "SessionsManagerTasks/SessionsManagerTask_CreateSession.h"
#include "SessionsManagerTasks/SessionsManagerTask_DestroySession.h"
#include "SessionsManagerTasks/SessionsManagerTask_FindSessions.h"
#include "SessionsManagerTasks/SessionsManagerTask_JoinSession.h"

TSharedPtr<FOnlineSessionsManager> FOnlineSessionsManager::Create()
{
	return MakeShared<FOnlineSessionsManager>();
}

FOnlineSessionsManager::FOnlineSessionsManager()
{
	SessionManagerState = MakeShared<FASessionManagerState>();
}

FOnlineSessionsManager::~FOnlineSessionsManager()
{
	bIsShuttingDown = true;

	if (CurrentTask.IsValid())
	{
		CurrentTask->OnExecuteCompletedDelegate.Unbind();
		CurrentTask->OnTaskAbortedDelegate.Unbind();
	}

	StopAllTasks();
}

void FOnlineSessionsManager::CreateSession(FName SessionName, const FOnlineSessionSettings& NewSessionSettings, FAOnCreateSessionCompleteDelegate OnCreateSessionComplete)
{
	AddNewTask(MakeShared<FASessionsManagerTask_CreateSession>(SessionManagerState, SessionName, NewSessionSettings, OnCreateSessionComplete));
}

void FOnlineSessionsManager::DestroySession(FName InSessionName, FAOnDestroySessionCompleteDelegate OnDestroySessionComplete)
{
	AddNewTask(MakeShared<FASessionsManagerTask_DestroySession>(SessionManagerState, InSessionName, OnDestroySessionComplete));
}

void FOnlineSessionsManager::FindSessions(const FASessionSearchSettings& SessionSearchSettings)
{
	AddNewTask(MakeShared<FASessionsManagerTask_FindSessions>(SessionManagerState, SessionSearchSettings));
}

void FOnlineSessionsManager::JoinSession(FName InSessionName, const FOnlineSessionSearchResult& DesiredSession, FAOnJoinSessionCompleteDelegate OnJoinSessionComplete)
{
	AddNewTask(MakeShared<FASessionsManagerTask_JoinSession>(SessionManagerState, InSessionName, DesiredSession, OnJoinSessionComplete));
}

void FOnlineSessionsManager::StopCurrentTask()
{
	if (CurrentTask.IsValid())
	{
		CurrentTask->Abort();
	}
}

void FOnlineSessionsManager::StopAllTasks()
{
	TasksQueue.Empty();
	StopCurrentTask();
}

void FOnlineSessionsManager::AddNewTask(TSharedPtr<FABaseSessionManagerTask> NewTask)
{
	if (bIsShuttingDown)
	{
		return;
	}

	NewTask->OnExecuteCompletedDelegate.BindSP(this, &FOnlineSessionsManager::OnCurrentTaskCompleted);
	NewTask->OnTaskAbortedDelegate.BindSP(this, &FOnlineSessionsManager::OnCurrentTaskAborted);
	TasksQueue.Emplace(MoveTemp(NewTask));

	if (!CurrentTask.IsValid())
	{
		ProcessNextTask();
	}
}

void FOnlineSessionsManager::ProcessNextTask()
{
	if (TasksQueue.IsEmpty())
	{
		return;
	}

	CurrentTask = TasksQueue[0];
	TasksQueue.RemoveAt(0);

	CurrentTask->Execute();
}

void FOnlineSessionsManager::OnCurrentTaskCompleted(bool bWasSuccessful)
{
	if (!DoesSharedInstanceExist())
	{
		UE_LOG(LogOnlineAdapter, Warning, TEXT("%s: Shared instance of sessions manager no longer exists"), ANSI_TO_TCHAR(__FUNCTION__));
		return;
	}

	TWeakPtr<FOnlineSessionsManager> CurrentWeakThis = AsShared();
	FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([CurrentWeakThis](float DeltaTime)
	{
		TSharedPtr<FOnlineSessionsManager> Self = CurrentWeakThis.Pin();
		if (!Self.IsValid() || Self->bIsShuttingDown)
		{
			return false;
		}

		if (Self->CurrentTask.IsValid())
		{
			Self->CurrentTask->OnExecuteCompletedDelegate.Unbind();
			Self->CurrentTask->OnTaskAbortedDelegate.Unbind();
		}

		Self->CurrentTask.Reset();
		Self->ProcessNextTask();

		return false;
	}));
}

void FOnlineSessionsManager::OnCurrentTaskAborted(bool bWasSuccessful)
{
	if (bIsShuttingDown)
	{
		return;
	}

	if (!bWasSuccessful)
	{
		UE_LOG(LogOnlineAdapter, Warning, TEXT("%s: Failed to abort current task"), ANSI_TO_TCHAR(__FUNCTION__));
		return;
	}

	if (CurrentTask.IsValid())
	{
		CurrentTask->OnExecuteCompletedDelegate.Unbind();
		CurrentTask->OnTaskAbortedDelegate.Unbind();
	}

	OnCurrentTaskCompleted(false);
}
