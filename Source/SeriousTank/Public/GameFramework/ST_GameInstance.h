#pragma once

#include "Engine/GameInstance.h"
#include "ST_GameInstance.generated.h"

USTRUCT(BlueprintType)
struct FLevelInfo
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
	TArray<FLevelInfo> LevelsInfo;

	UPROPERTY(EditAnywhere)
	FVehicleInfo VehicleInfo; 

public:
	const TArray<FLevelInfo>& GetLevelsInfo() const 
	{ 
		return LevelsInfo; 
	}

	const FLevelInfo* GetLevelInfo(int32 Index) const
	{
		return Index >= 0 && Index < LevelsInfo.Num() ? &LevelsInfo[Index] : nullptr;
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
