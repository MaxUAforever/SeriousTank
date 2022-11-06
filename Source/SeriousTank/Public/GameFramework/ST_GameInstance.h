#pragma once

#include "Engine/GameInstance.h"
#include "ST_GameInstance.generated.h"

USTRUCT(BlueprintType)
struct FGameplayLevelInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly)
	FString LevelName;
};

class AST_BaseVehicle;
class ABaseWeapon;

USTRUCT(BlueprintType)
struct FVehicleInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<AST_BaseVehicle> VehicleClass;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<ABaseWeapon>> WeaponClasses;
};

UCLASS()
class SERIOUSTANK_API UST_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TArray<FGameplayLevelInfo> LevelsInfo;

	UPROPERTY(EditDefaultsOnly)
	FString MainMenuLevelName;

	UPROPERTY(EditAnywhere)
	FVehicleInfo VehicleInfo; 

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

	TSubclassOf<AST_BaseVehicle> GetVehicleClass() const 
	{
		return VehicleInfo.VehicleClass; 
	};

	TSubclassOf<ABaseWeapon> GetWeaponClass(int32 Index) const 
	{
		return Index >= 0 && Index < VehicleInfo.WeaponClasses.Num() ? VehicleInfo.WeaponClasses[Index] : nullptr;
	};

	void SetVehicle(TSubclassOf<AST_BaseVehicle> NewVehicleClass);
	bool SetWeapon(TSubclassOf<ABaseWeapon> NewWeaponClass, int32 WeaponIndex);
};
