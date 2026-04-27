#include "SessionsManagerTasks/SessionsManagerTask_JoinSession.h"

#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystem.h"

FASessionsManagerTask_JoinSession::FASessionsManagerTask_JoinSession(TSharedPtr<FASessionManagerState> InSessionManagerState, FName InSessionName, const FOnlineSessionSearchResult& InDesiredSession, FAOnJoinSessionCompleteDelegate InOnJoinSessionComplete)
	: FABaseSessionManagerTask(InSessionManagerState)
	, SessionName(InSessionName)
	, DesiredSession(InDesiredSession)
	, OnJoinSessionComplete(InOnJoinSessionComplete)
{}

FASessionsManagerTask_JoinSession::~FASessionsManagerTask_JoinSession()
{
	ClearDelegates();
}

bool FASessionsManagerTask_JoinSession::CanBeExecuted() const
{
	const IOnlineSubsystem* PlatformOnlineSubsystem = IOnlineSubsystem::GetByPlatform();
	const IOnlineSessionPtr PlatformSessionInterface = PlatformOnlineSubsystem != nullptr ? PlatformOnlineSubsystem->GetSessionInterface() : nullptr;
	if (PlatformSessionInterface == nullptr)
	{
		UE_LOG(LogOnlineAdapter, Warning, TEXT("%s: Failed to get session interface for platform %s"), ANSI_TO_TCHAR(__FUNCTION__), *FString(PlatformOnlineSubsystem != nullptr ? PlatformOnlineSubsystem->GetSubsystemName().ToString() : TEXT("None")));
		return false;
	}

	if (IsInSession(SessionName))
	{
		UE_LOG(LogOnlineAdapter, Warning, TEXT("%s: Already in a session %s"), ANSI_TO_TCHAR(__FUNCTION__), *SessionName.ToString());
		return false;
	}

	return true;
}

void FASessionsManagerTask_JoinSession::InternalExecute()
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

	JoinSessionDelegateHandle = PlatformSessionInterface->AddOnJoinSessionCompleteDelegate_Handle(FOnJoinSessionCompleteDelegate::CreateSP(this, &FASessionsManagerTask_JoinSession::OnSessionJoined));
	PlatformSessionInterface->JoinSession(SessionManagerState->CurrentLocalUserNum, SessionName, DesiredSession);
}

void FASessionsManagerTask_JoinSession::OnExecuteCompleted(bool bWasSuccessful)
{
	ClearDelegates();

	OnExecuteCompletedDelegate.ExecuteIfBound(bWasSuccessful);
	OnJoinSessionComplete.ExecuteIfBound(bWasSuccessful);
}

void FASessionsManagerTask_JoinSession::OnSessionJoined(FName InSessionName, EOnJoinSessionCompleteResult::Type JoinResult)
{
	if (JoinResult != EOnJoinSessionCompleteResult::Success)
	{
		UE_LOG(LogOnlineAdapter, Warning, TEXT("%s: Failed to join session %s."), ANSI_TO_TCHAR(__FUNCTION__), *InSessionName.ToString());
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

	SessionManagerState->CurrentSessionNames.Add(SessionName);
	OnExecuteCompleted(true);
}

void FASessionsManagerTask_JoinSession::ClearDelegates()
{
	const IOnlineSubsystem* PlatformOnlineSubsystem = IOnlineSubsystem::GetByPlatform();
	const IOnlineSessionPtr PlatformSessionInterface = PlatformOnlineSubsystem != nullptr ? PlatformOnlineSubsystem->GetSessionInterface() : nullptr;
	if (!PlatformSessionInterface)
	{
		UE_LOG(LogOnlineAdapter, Warning, TEXT("%s: Failed to get session interface for platform %s"), ANSI_TO_TCHAR(__FUNCTION__), *FString(PlatformOnlineSubsystem != nullptr ? PlatformOnlineSubsystem->GetSubsystemName().ToString() : TEXT("None")));
		return;
	}

	if (JoinSessionDelegateHandle.IsValid())
	{
		PlatformSessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionDelegateHandle);
		JoinSessionDelegateHandle.Reset();
	}
}