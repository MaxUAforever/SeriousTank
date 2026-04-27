#include "SessionsManagerTasks/BaseSessionsManagerTask.h"

#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystem.h"

FABaseSessionManagerTask::FABaseSessionManagerTask(TSharedPtr<FASessionManagerState> InSessionManagerState)
	: CurrentSessionManagerState(InSessionManagerState)
{}

const FNamedOnlineSession* FABaseSessionManagerTask::GetSession(FName SessionName) const
{
	TSharedPtr<FASessionManagerState> CurrentSessionState = GetSessionManagerState();
	if (!CurrentSessionState.IsValid())
	{
		UE_LOG(LogOnlineAdapter, Warning, TEXT("%s: Session manager state is not initialized"), ANSI_TO_TCHAR(__FUNCTION__));
		return nullptr;
	}

	const IOnlineSubsystem* PlatformOnlineSubsystem = IOnlineSubsystem::GetByPlatform();
	const IOnlineSessionPtr PlatformSessionInterface = PlatformOnlineSubsystem != nullptr ? PlatformOnlineSubsystem->GetSessionInterface() : nullptr;
	if (PlatformSessionInterface == nullptr)
	{
		UE_LOG(LogOnlineAdapter, Warning, TEXT("%s: Failed to get session interface for platform %s"), ANSI_TO_TCHAR(__FUNCTION__), *FString(PlatformOnlineSubsystem != nullptr ? PlatformOnlineSubsystem->GetSubsystemName().ToString() : TEXT("None")));
		return nullptr;
	}

	return PlatformSessionInterface->GetNamedSession(SessionName);
}

bool FABaseSessionManagerTask::IsInSession(FName SessionName) const
{
	return GetSession(SessionName) != nullptr;
}

void FABaseSessionManagerTask::Execute()
{
	if (!CanBeExecuted())
	{
		UE_LOG(LogOnlineAdapter, Warning, TEXT("%s: Task cannot be executed in its current state"), ANSI_TO_TCHAR(__FUNCTION__));
		OnExecuteCompleted(false);
		return;
	}

	InternalExecute();
}

void FABaseSessionManagerTask::Abort()
{
	if (!CanBeAborted())
	{
		UE_LOG(LogOnlineAdapter, Log, TEXT("%s: Task cannot be aborted in its current state"), ANSI_TO_TCHAR(__FUNCTION__));
		OnAbortCompleted(false);
		return;
	}

	InternalAbort();
}

void FABaseSessionManagerTask::OnExecuteCompleted(bool bWasSuccessful)
{
	OnExecuteCompletedDelegate.ExecuteIfBound(bWasSuccessful);
}

void FABaseSessionManagerTask::OnAbortCompleted(bool bWasSuccessful)
{
	OnTaskAbortedDelegate.ExecuteIfBound(bWasSuccessful);
}
