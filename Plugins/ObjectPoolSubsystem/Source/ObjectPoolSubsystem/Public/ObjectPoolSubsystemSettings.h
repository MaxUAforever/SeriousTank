#pragma once

#include "Engine/DeveloperSettings.h"
#include "ObjectPoolSubsystemSettings.generated.h"

USTRUCT(BlueprintType)
struct FObjectPoolInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> ObjectClass;

	UPROPERTY(EditDefaultsOnly, Meta = (ClampMin = "0"))
	int32 ObjectsAmount;

	UPROPERTY(EditDefaultsOnly)
	TArray<FString> AllowedMaps;

	UPROPERTY(EditDefaultsOnly)
	TArray<FString> RestrictedMaps;
};

UCLASS(config = Game, defaultconfig, meta = (DisplayName = "Object Pool Subsystem Settings"))
class OBJECTPOOLSUBSYSTEM_API UObjectPoolSubsystemSettings : public UDeveloperSettings
{
    GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Data")
	TArray<FObjectPoolInfo> ObjectsPoolInfo;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Data")
	TArray<FString> CommonRestrictedMaps;
};
