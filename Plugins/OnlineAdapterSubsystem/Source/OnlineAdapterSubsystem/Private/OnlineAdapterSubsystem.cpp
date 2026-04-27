#include "OnlineAdapterSubsystem.h"

void UOnlineAdapterSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	IdentityManager = MakeShared<FOnlineIdentityManager, ESPMode::ThreadSafe>();
	SessionsManager = FOnlineSessionsManager::Create();
}