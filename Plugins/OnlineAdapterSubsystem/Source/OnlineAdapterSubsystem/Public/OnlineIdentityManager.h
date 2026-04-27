#pragma once

#include "OnlineBaseManager.h"
#include "OnlineAdapterTypes.h"
#include "Delegates/IDelegateInstance.h"

DECLARE_DELEGATE_TwoParams(FAOnLoginCompleteDelegate, bool /*bWasSuccessful*/, const FALocalLoggedUserInfo& /*LocalUserInfo*/);
DECLARE_DELEGATE_OneParam(FAOnLogoutCompleteDelegate, bool /*bWasSuccessful*/);

class FOnlineIdentityManager : public FOnlineBaseManager, public TSharedFromThis<FOnlineIdentityManager>
{
public:
	FOnlineIdentityManager() = default;
	~FOnlineIdentityManager();

public:
	void Login(FAOnLoginCompleteDelegate OnLoginComplete = FAOnLoginCompleteDelegate());
	void Logout(FAOnLogoutCompleteDelegate OnLogoutComplete = FAOnLogoutCompleteDelegate());
	bool IsLoggedIn() const;

	inline const FALocalLoggedUserInfo& GetLocalLoggedUserInfo() const { return LocalUserInfo; }

private:
	void OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);
	void OnLogoutComplete(int32 LocalUserNum, bool bWasSuccessful);

	bool IsOperationInProgress() const;
	void ClearDelegates();

private:
	FALocalLoggedUserInfo LocalUserInfo;

	FDelegateHandle OnLoginCompleteDelegateHandle;
	FDelegateHandle OnLogoutCompleteDelegateHandle;

	FAOnLoginCompleteDelegate OnLoginCompleteDelegate;
	FAOnLogoutCompleteDelegate OnLogoutCompleteDelegate;
};
