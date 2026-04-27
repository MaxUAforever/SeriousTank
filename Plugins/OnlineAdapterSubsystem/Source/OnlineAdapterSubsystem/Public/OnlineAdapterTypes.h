#pragma once

#include "OnlineSessionSettings.h"

DECLARE_LOG_CATEGORY_EXTERN(LogOnlineAdapter, Log, All);

DECLARE_DELEGATE_OneParam(FAOnCreateSessionCompleteDelegate, bool /*bWasSuccessful*/);
DECLARE_DELEGATE_OneParam(FAOnJoinSessionCompleteDelegate, bool /*bWasSuccessful*/);
DECLARE_DELEGATE_OneParam(FAOnDestroySessionCompleteDelegate, bool /*bWasSuccessful*/);
DECLARE_DELEGATE_TwoParams(FAOnFindSessionsCompleteDelegate, bool /*bWasSuccessful*/, const TArray<FOnlineSessionSearchResult>& /*SearchResults*/);

struct FALocalLoggedUserInfo
{
	int32 LocalUserNum = -1;
	FUniqueNetIdPtr LocalUserId;

public:
	inline bool IsValid() const
	{
		return LocalUserNum != -1 && LocalUserId.IsValid();
	}
};

struct FASessionManagerState
{
	TSet<FName> CurrentSessionNames;
	int32 CurrentLocalUserNum = 0;
};

struct FASessionSearchSettings
{
	FOnlineSearchSettings QuerySettings;

	int32 MaxSearchResults;
	bool bIsLanQuery;
};
