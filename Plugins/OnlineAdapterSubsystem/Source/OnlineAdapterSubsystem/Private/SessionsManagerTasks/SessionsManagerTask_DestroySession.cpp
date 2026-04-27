#include "SessionsManagerTasks/SessionsManagerTask_DestroySession.h"

#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystem.h"

FASessionsManagerTask_DestroySession::FASessionsManagerTask_DestroySession(TSharedPtr<FASessionManagerState> InSessionManagerState, FName InSessionName, FAOnDestroySessionCompleteDelegate InOnDestroySessionComplete)
	: FABaseSessionManagerTask(InSessionManagerState)
	, SessionName(InSessionName)
	, OnDestroySessionCompleteDelegate(InOnDestroySessionComplete)
{}

FASessionsManagerTask_DestroySession::~FASessionsManagerTask_DestroySession()
{
	ClearDelegates();
}

bool FASessionsManagerTask_DestroySession::CanBeExecuted() const
{
	const IOnlineSubsystem* PlatformOnlineSubsystem = IOnlineSubsystem::GetByPlatform();
	const IOnlineSessionPtr PlatformSessionInterface = PlatformOnlineSubsystem != nullptr ? PlatformOnlineSubsystem->GetSessionInterface() : nullptr;
	if (PlatformSessionInterface == nullptr)
	{
		UE_LOG(LogOnlineAdapter, Warning, TEXT("%s: Failed to get session interface for platform %s"), ANSI_TO_TCHAR(__FUNCTION__), *FString(PlatformOnlineSubsystem != nullptr ? PlatformOnlineSubsystem->GetSubsystemName().ToString() : TEXT("None")));
		return false;
	}

	if (!IsInSession(SessionName))
	{
		UE_LOG(LogOnlineAdapter, Warning, TEXT("%s: No current session to destroy"), ANSI_TO_TCHAR(__FUNCTION__));
		return false;
	}

	return true;
}

void FASessionsManagerTask_DestroySession::InternalExecute()
{
	const IOnlineSubsystem* PlatformOnlineSubsystem = IOnlineSubsystem::GetByPlatform();
	const IOnlineSessionPtr PlatformSessionInterface = PlatformOnlineSubsystem != nullptr ? PlatformOnlineSubsystem->GetSessionInterface() : nullptr;
	check(PlatformSessionInterface != nullptr);

	TSharedPtr<FASessionManagerState> SessionManagerState = GetSessionManagerState();
	if (!SessionManagerState.IsValid())
	{
		UE_LOG(LogOnlineAdapter, Warning, TEXT("%s: Session manager state is not initialized"), ANSI_TO_TCHAR(__FUNCTION__));
		OnExecuteCompleted(false);
		return;
	}

	DestroySessionDelegateHandle = PlatformSessionInterface->AddOnDestroySessionCompleteDelegate_Handle(FOnDestroySessionCompleteDelegate::CreateRaw(this, &FASessionsManagerTask_DestroySession::OnSessionDestroyed));
	PlatformSessionInterface->DestroySession(SessionName);
}

void FASessionsManagerTask_DestroySession::OnExecuteCompleted(bool bWasSuccessful)
{
	ClearDelegates();

	OnDestroySessionCompleteDelegate.ExecuteIfBound(bWasSuccessful);
	OnExecuteCompletedDelegate.ExecuteIfBound(bWasSuccessful);
}

void FASessionsManagerTask_DestroySession::OnSessionDestroyed(FName InSessionName, bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		UE_LOG(LogOnlineAdapter, Warning, TEXT("%s: Failed to create session %s"), ANSI_TO_TCHAR(__FUNCTION__), *SessionName.ToString());
		OnExecuteCompleted(false);
		return;
	}

	TSharedPtr<FASessionManagerState> SessionManagerState = GetSessionManagerState();
	if (!SessionManagerState.IsValid())
	{
		UE_LOG(LogOnlineAdapter, Warning, TEXT("%s: Session manager state is not initialized"), ANSI_TO_TCHAR(__FUNCTION__));
		OnExecuteCompleted(false);
		return;
	}

	SessionManagerState->CurrentSessionNames.Remove(InSessionName);
	OnExecuteCompleted(true);
}

void FASessionsManagerTask_DestroySession::ClearDelegates()
{
	const IOnlineSubsystem* PlatformOnlineSubsystem = IOnlineSubsystem::GetByPlatform();
	const IOnlineSessionPtr PlatformSessionInterface = PlatformOnlineSubsystem != nullptr ? PlatformOnlineSubsystem->GetSessionInterface() : nullptr;
	if (!PlatformSessionInterface)
	{
		UE_LOG(LogOnlineAdapter, Warning, TEXT("%s: Failed to get session interface for platform %s"), ANSI_TO_TCHAR(__FUNCTION__), *FString(PlatformOnlineSubsystem != nullptr ? PlatformOnlineSubsystem->GetSubsystemName().ToString() : TEXT("None")));
		return;
	}

	if (DestroySessionDelegateHandle.IsValid())
	{
		PlatformSessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionDelegateHandle);
		DestroySessionDelegateHandle.Reset();
	}
}
