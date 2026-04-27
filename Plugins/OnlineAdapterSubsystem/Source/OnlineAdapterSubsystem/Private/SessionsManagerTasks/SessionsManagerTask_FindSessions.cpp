#include "SessionsManagerTasks/SessionsManagerTask_FindSessions.h"

#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystem.h"

FASessionsManagerTask_FindSessions::FASessionsManagerTask_FindSessions(TSharedPtr<FASessionManagerState> InSessionManagerState, const FASessionSearchSettings& InSearchSettings, FAOnFindSessionsCompleteDelegate InOnFindSessionsComplete)
	: FABaseSessionManagerTask(InSessionManagerState)
	, SearchSettings(InSearchSettings)
	, OnFindSessionsCompleteDelegate(InOnFindSessionsComplete)
{}

FASessionsManagerTask_FindSessions::~FASessionsManagerTask_FindSessions()
{
	ClearDelegates();
}

bool FASessionsManagerTask_FindSessions::CanBeExecuted() const
{
	const IOnlineSubsystem* PlatformOnlineSubsystem = IOnlineSubsystem::GetByPlatform();
	const IOnlineSessionPtr PlatformSessionInterface = PlatformOnlineSubsystem != nullptr ? PlatformOnlineSubsystem->GetSessionInterface() : nullptr;
	if (PlatformSessionInterface == nullptr)
	{
		UE_LOG(LogOnlineAdapter, Warning, TEXT("%s: Failed to get session interface for platform %s"), ANSI_TO_TCHAR(__FUNCTION__), *FString(PlatformOnlineSubsystem != nullptr ? PlatformOnlineSubsystem->GetSubsystemName().ToString() : TEXT("None")));
		return false;
	}

	return true;
}

bool FASessionsManagerTask_FindSessions::CanBeAborted() const
{
	const IOnlineSubsystem* PlatformOnlineSubsystem = IOnlineSubsystem::GetByPlatform();
	const IOnlineSessionPtr PlatformSessionInterface = PlatformOnlineSubsystem != nullptr ? PlatformOnlineSubsystem->GetSessionInterface() : nullptr;
	if (PlatformSessionInterface == nullptr)
	{
		UE_LOG(LogOnlineAdapter, Warning, TEXT("%s: Failed to get session interface for platform %s"), ANSI_TO_TCHAR(__FUNCTION__), *FString(PlatformOnlineSubsystem != nullptr ? PlatformOnlineSubsystem->GetSubsystemName().ToString() : TEXT("None")));
		return false;
	}

	if (!SessionSearch.IsValid() || SessionSearch->SearchState != EOnlineAsyncTaskState::InProgress)
	{
		UE_LOG(LogOnlineAdapter, Warning, TEXT("%s: No active session search to cancel"), ANSI_TO_TCHAR(__FUNCTION__));
		return false;
	}

	return true;
}

void FASessionsManagerTask_FindSessions::InternalExecute()
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
	
	SessionSearch = MakeShared<FOnlineSessionSearch>();
	SessionSearch->QuerySettings = SearchSettings.QuerySettings;
	SessionSearch->bIsLanQuery = SearchSettings.bIsLanQuery;
	SessionSearch->MaxSearchResults = SearchSettings.MaxSearchResults;

	FindSessionsDelegateHandle = PlatformSessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FOnFindSessionsCompleteDelegate::CreateSP(this, &FASessionsManagerTask_FindSessions::OnSessionsFound));
	PlatformSessionInterface->FindSessions(SessionManagerState->CurrentLocalUserNum, SessionSearch.ToSharedRef());
}

void FASessionsManagerTask_FindSessions::InternalAbort()
{
	const IOnlineSubsystem* PlatformOnlineSubsystem = IOnlineSubsystem::GetByPlatform();
	const IOnlineSessionPtr PlatformSessionInterface = PlatformOnlineSubsystem != nullptr ? PlatformOnlineSubsystem->GetSessionInterface() : nullptr;
	check(PlatformSessionInterface != nullptr);

	CancelFindSessionsDelegateHandle = PlatformSessionInterface->AddOnCancelFindSessionsCompleteDelegate_Handle(FOnCancelFindSessionsCompleteDelegate::CreateSP(this, &FASessionsManagerTask_FindSessions::OnAbortCompleted));
	PlatformSessionInterface->CancelFindSessions();
}

void FASessionsManagerTask_FindSessions::OnExecuteCompleted(bool bWasSuccessful)
{
	ClearDelegates();

	OnExecuteCompletedDelegate.ExecuteIfBound(bWasSuccessful);
	OnFindSessionsCompleteDelegate.ExecuteIfBound(bWasSuccessful, bWasSuccessful ? SessionSearch->SearchResults : TArray<FOnlineSessionSearchResult>());

	SessionSearch.Reset();
}

void FASessionsManagerTask_FindSessions::OnAbortCompleted(bool bWasSuccessful)
{
	ClearDelegates();

	OnExecuteCompletedDelegate.ExecuteIfBound(false);
	SessionSearch.Reset();
}

void FASessionsManagerTask_FindSessions::OnSessionsFound(bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		UE_LOG(LogOnlineAdapter, Warning, TEXT("%s: Find sessions failed"), ANSI_TO_TCHAR(__FUNCTION__));
		OnExecuteCompleted(false);
		return;
	}

	OnExecuteCompleted(true);
}

void FASessionsManagerTask_FindSessions::ClearDelegates()
{
	const IOnlineSubsystem* PlatformOnlineSubsystem = IOnlineSubsystem::GetByPlatform();
	const IOnlineSessionPtr PlatformSessionInterface = PlatformOnlineSubsystem != nullptr ? PlatformOnlineSubsystem->GetSessionInterface() : nullptr;
	if (PlatformSessionInterface == nullptr)
	{
		UE_LOG(LogOnlineAdapter, Warning, TEXT("%s: Failed to get session interface for platform %s"), ANSI_TO_TCHAR(__FUNCTION__), *FString(PlatformOnlineSubsystem != nullptr ? PlatformOnlineSubsystem->GetSubsystemName().ToString() : TEXT("None")));
		return;
	}

	if (FindSessionsDelegateHandle.IsValid())
	{
		PlatformSessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsDelegateHandle);
		FindSessionsDelegateHandle.Reset();
	}

	if (CancelFindSessionsDelegateHandle.IsValid())
	{
		PlatformSessionInterface->ClearOnCancelFindSessionsCompleteDelegate_Handle(CancelFindSessionsDelegateHandle);
		CancelFindSessionsDelegateHandle.Reset();
	}
}
