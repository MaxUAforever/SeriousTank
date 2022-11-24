#pragma once

#include "Engine/GameInstance.h"

#include "Core/ST_CoreTypes.h"

#include "ST_GameInstance.generated.h"

class AST_BaseVehicle;
class ABaseWeapon;
class UAudioComponent;

UCLASS()
class SERIOUSTANK_API UST_GameInstance : public UGameInstance
{
	GENERATED_BODY()
    
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Levels")
	TArray<FGameplayLevelInfo> LevelsInfo;

	UPROPERTY(EditDefaultsOnly, Category = "Levels")
	FString MainMenuLevelName;

public:
	const TArray<FGameplayLevelInfo>& GetLevelsInfo() const
	{ 
		return LevelsInfo; 
	}

	const FGameplayLevelInfo* GetLevelInfo(int32 Index) const
	{
		return Index >= 0 && Index < LevelsInfo.Num() ? &LevelsInfo[Index] : nullptr;
	}

	const FString GetMainMenuLevelName() const
	{
		return MainMenuLevelName;
	}
};
