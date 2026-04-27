#include "OnlineIdentityManager.h"

#include "Interfaces/OnlineIdentityInterface.h"
#include "OnlineSubsystem.h"

FOnlineIdentityManager::~FOnlineIdentityManager()
{
	ClearDelegates();
}

void FOnlineIdentityManager::Login(FAOnLoginCompleteDelegate OnLoginComplete)
{
	const IOnlineSubsystem* PlatformOnlineSubsystem = IOnlineSubsystem::GetByPlatform();
	const IOnlineIdentityPtr PlatformIdentityInterface = PlatformOnlineSubsystem != nullptr ? PlatformOnlineSubsystem->GetIdentityInterface() : nullptr;
	if (PlatformIdentityInterface == nullptr)
	{
		UE_LOG(LogOnlineAdapter, Warning, TEXT("%s: Failed to get identity interface for platform %s"), ANSI_TO_TCHAR(__FUNCTION__), *FString(PlatformOnlineSubsystem != nullptr ? PlatformOnlineSubsystem->GetSubsystemName().ToString() : TEXT("None")));
		OnLoginComplete.ExecuteIfBound(false, FALocalLoggedUserInfo());
		return;
	}

	if (IsOperationInProgress())
	{
		UE_LOG(LogOnlineAdapter, Warning, TEXT("%s: A login or logout operation is already in progress"), ANSI_TO_TCHAR(__FUNCTION__));
		OnLoginComplete.ExecuteIfBound(false, FALocalLoggedUserInfo());
		return;
	}

	const int32 LocalUserNum = GetDefaultLocalUserNum();
	if (IsLoggedIn())
	{
		UE_LOG(LogOnlineAdapter, Warning, TEXT("%s: Local user %d is already logged in"), ANSI_TO_TCHAR(__FUNCTION__), LocalUserNum);
		OnLoginComplete.ExecuteIfBound(true, LocalUserInfo);
		return;
	}
	
	OnLoginCompleteDelegate = OnLoginComplete;
	OnLoginCompleteDelegateHandle = PlatformIdentityInterface->AddOnLoginCompleteDelegate_Handle(LocalUserNum, FOnLoginCompleteDelegate::CreateSP(this, &FOnlineIdentityManager::OnLoginComplete));
	PlatformIdentityInterface->Login(LocalUserNum, FOnlineAccountCredentials());
	return;
}

void FOnlineIdentityManager::Logout(FAOnLogoutCompleteDelegate OnLogoutComplete)
{
	const IOnlineSubsystem* PlatformOnlineSubsystem = IOnlineSubsystem::GetByPlatform();
	const IOnlineIdentityPtr PlatformIdentityInterface = PlatformOnlineSubsystem != nullptr ? PlatformOnlineSubsystem->GetIdentityInterface() : nullptr;
	if (PlatformIdentityInterface == nullptr)
	{
		UE_LOG(LogOnlineAdapter, Warning, TEXT("%s: Failed to get identity interface for platform %s"), ANSI_TO_TCHAR(__FUNCTION__), *FString(PlatformOnlineSubsystem != nullptr ? PlatformOnlineSubsystem->GetSubsystemName().ToString() : TEXT("None")));
		OnLogoutComplete.ExecuteIfBound(false);
		return;
	}

	if (IsOperationInProgress())
	{
		UE_LOG(LogOnlineAdapter, Warning, TEXT("%s: A login or logout operation is already in progress"), ANSI_TO_TCHAR(__FUNCTION__));
		OnLogoutComplete.ExecuteIfBound(false);
		return;
	}

	if (!IsLoggedIn())
	{
		UE_LOG(LogOnlineAdapter, Warning, TEXT("%s: No local user is currently logged in"), ANSI_TO_TCHAR(__FUNCTION__));
		OnLogoutComplete.ExecuteIfBound(true);
		return;
	}

	const int32 LocalUserNum = GetDefaultLocalUserNum();
	OnLogoutCompleteDelegateHandle = PlatformIdentityInterface->AddOnLogoutCompleteDelegate_Handle(LocalUserNum, FOnLogoutCompleteDelegate::CreateSP(this, &FOnlineIdentityManager::OnLogoutComplete));
	PlatformIdentityInterface->Logout(GetDefaultLocalUserNum());
}

bool FOnlineIdentityManager::IsLoggedIn() const
{
	const IOnlineSubsystem* PlatformOnlineSubsystem = IOnlineSubsystem::GetByPlatform();
	const IOnlineIdentityPtr PlatformIdentityInterface = PlatformOnlineSubsystem != nullptr ? PlatformOnlineSubsystem->GetIdentityInterface() : nullptr;
	if (PlatformIdentityInterface == nullptr)
	{
		UE_LOG(LogOnlineAdapter, Warning, TEXT("%s: Failed to get identity interface for platform %s"), ANSI_TO_TCHAR(__FUNCTION__), *FString(PlatformOnlineSubsystem != nullptr ? PlatformOnlineSubsystem->GetSubsystemName().ToString() : TEXT("None")));
		return false;
	}

	return PlatformIdentityInterface->GetLoginStatus(GetDefaultLocalUserNum()) == ELoginStatus::LoggedIn;
}

void FOnlineIdentityManager::OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error)
{
	ClearDelegates();

	if (!bWasSuccessful)
	{
		UE_LOG(LogOnlineAdapter, Warning, TEXT("%s: Login failed for local user %d with error: %s"), ANSI_TO_TCHAR(__FUNCTION__), LocalUserNum, *Error);
		OnLoginCompleteDelegate.ExecuteIfBound(false, FALocalLoggedUserInfo());
		OnLoginCompleteDelegate.Unbind();
		return;
	}

	LocalUserInfo.LocalUserNum = LocalUserNum;
	LocalUserInfo.LocalUserId = UserId.AsShared();

	OnLoginCompleteDelegate.ExecuteIfBound(true, LocalUserInfo);
	OnLoginCompleteDelegate.Unbind();
}

void FOnlineIdentityManager::OnLogoutComplete(int32 LocalUserNum, bool bWasSuccessful)
{
	ClearDelegates();

	if (!bWasSuccessful)
	{
		UE_LOG(LogOnlineAdapter, Warning, TEXT("%s: Logout failed for local user %d"), ANSI_TO_TCHAR(__FUNCTION__), LocalUserNum);
		OnLogoutCompleteDelegate.ExecuteIfBound(false);
		OnLogoutCompleteDelegate.Unbind();
		return;
	}

	LocalUserInfo = FALocalLoggedUserInfo();

	OnLogoutCompleteDelegate.ExecuteIfBound(true);
	OnLogoutCompleteDelegate.Unbind();
}

bool FOnlineIdentityManager::IsOperationInProgress() const
{
	return OnLoginCompleteDelegateHandle.IsValid() || OnLogoutCompleteDelegateHandle.IsValid();
}

void FOnlineIdentityManager::ClearDelegates()
{
	const IOnlineSubsystem* PlatformOnlineSubsystem = IOnlineSubsystem::GetByPlatform();
	const IOnlineIdentityPtr PlatformIdentityInterface = PlatformOnlineSubsystem != nullptr ? PlatformOnlineSubsystem->GetIdentityInterface() : nullptr;
	if (PlatformIdentityInterface == nullptr)
	{
		UE_LOG(LogOnlineAdapter, Warning, TEXT("%s: Failed to get identity interface for platform %s"), ANSI_TO_TCHAR(__FUNCTION__), *FString(PlatformOnlineSubsystem != nullptr ? PlatformOnlineSubsystem->GetSubsystemName().ToString() : TEXT("None")));
		return;
	}

	if (OnLoginCompleteDelegateHandle.IsValid())
	{
		PlatformIdentityInterface->ClearOnLoginCompleteDelegate_Handle(GetDefaultLocalUserNum(), OnLoginCompleteDelegateHandle);
		OnLoginCompleteDelegateHandle.Reset();
	}

	if (OnLogoutCompleteDelegateHandle.IsValid())
	{
		PlatformIdentityInterface->ClearOnLogoutCompleteDelegate_Handle(GetDefaultLocalUserNum(), OnLogoutCompleteDelegateHandle);
		OnLogoutCompleteDelegateHandle.Reset();
	}
}


