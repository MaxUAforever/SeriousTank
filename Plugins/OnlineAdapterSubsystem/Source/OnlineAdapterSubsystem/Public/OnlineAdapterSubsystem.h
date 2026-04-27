#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "OnlineIdentityManager.h"
#include "OnlineSessionsManager.h"
#include "OnlineAdapterSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class ONLINEADAPTERSUBSYSTEM_API UOnlineAdapterSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	TSharedPtr<FOnlineIdentityManager> GetIdentityManager() { return IdentityManager; }
	TSharedPtr<FOnlineSessionsManager> GetSessionsManager() { return SessionsManager; }

private:
	TSharedPtr<FOnlineIdentityManager> IdentityManager;
	TSharedPtr<FOnlineSessionsManager> SessionsManager;
};
