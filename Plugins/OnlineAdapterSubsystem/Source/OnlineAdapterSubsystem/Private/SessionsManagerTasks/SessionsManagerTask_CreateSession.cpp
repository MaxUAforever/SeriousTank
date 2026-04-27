#include "SessionsManagerTasks/SessionsManagerTask_CreateSession.h"

#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystem.h"

FASessionsManagerTask_CreateSession::FASessionsManagerTask_CreateSession(TSharedPtr<FASessionManagerState> InSessionManagerState, FName InSessionName, const FOnlineSessionSettings& InSessionSettings, FAOnCreateSessionCompleteDelegate InOnCreateSessionComplete)
	: FABaseSessionManagerTask(InSessionManagerState)
	, SessionName(InSessionName)
	, SessionSettings(InSessionSettings)
	, OnCreateSessionCompleteDelegate(InOnCreateSessionComplete)
{}

FASessionsManagerTask_CreateSession::~FASessionsManagerTask_CreateSession()
{
	ClearDelegates();
}

bool FASessionsManagerTask_CreateSession::CanBeExecuted() const
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

void FASessionsManagerTask_CreateSession::InternalExecute()
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

	const int32 DefaultLocalUserNum = SessionManagerState->CurrentLocalUserNum;

	CreateSessionDelegateHandle = PlatformSessionInterface->AddOnCreateSessionCompleteDelegate_Handle(FOnCreateSessionCompleteDelegate::CreateSP(this, &FASessionsManagerTask_CreateSession::OnSessionCreated));
	PlatformSessionInterface->CreateSession(DefaultLocalUserNum, SessionName, SessionSettings);
}

void FASessionsManagerTask_CreateSession::OnExecuteCompleted(bool bWasSuccessful)
{
	ClearDelegates();
	
	OnExecuteCompletedDelegate.ExecuteIfBound(bWasSuccessful);
	OnCreateSessionCompleteDelegate.ExecuteIfBound(bWasSuccessful);
}

void FASessionsManagerTask_CreateSession::OnSessionCreated(FName InSessionName, bool bWasSuccessful)
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

	SessionManagerState->CurrentSessionNames.Add(SessionName);
	OnExecuteCompleted(true);
}

void FASessionsManagerTask_CreateSession::ClearDelegates()
{
	const IOnlineSubsystem* PlatformOnlineSubsystem = IOnlineSubsystem::GetByPlatform();
	const IOnlineSessionPtr PlatformSessionInterface = PlatformOnlineSubsystem != nullptr ? PlatformOnlineSubsystem->GetSessionInterface() : nullptr;
	if (!PlatformSessionInterface)
	{
		UE_LOG(LogOnlineAdapter, Warning, TEXT("%s: Failed to get session interface for platform %s"), ANSI_TO_TCHAR(__FUNCTION__), *FString(PlatformOnlineSubsystem != nullptr ? PlatformOnlineSubsystem->GetSubsystemName().ToString() : TEXT("None")));
		return;
	}

	if (CreateSessionDelegateHandle.IsValid())
	{
		PlatformSessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionDelegateHandle);
		CreateSessionDelegateHandle.Reset();
	}
}
