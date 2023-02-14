#pragma once

#include "UObject/Object.h"
#include "ST_TargetRespawnManager.generated.h"

class AST_ShootTarget;
class AST_TargetSpawningVolume;

UCLASS(BlueprintType)
class SERIOUSTANK_API UST_TargetRespawnManager : public UObject
{
	GENERATED_BODY()

protected:
    UPROPERTY(EditAnywhere, Category = "Settings")
    TSubclassOf<AST_TargetSpawningVolume> SpawnVolumeClass;

	UPROPERTY(EditAnywhere, Category = "Settings")
    TSubclassOf<AST_ShootTarget> ShootTargetClass;

private:
	TArray<AST_TargetSpawningVolume*> SpawnVolumes;

	float SpawningHeight = 50.f;

public:
	void SetSpawnVolumeClass(TSubclassOf<AST_TargetSpawningVolume> NewSpawnVolumeClass);
	void SetShootTargetClass(TSubclassOf<AST_ShootTarget> NewShootTargetClass) { ShootTargetClass = NewShootTargetClass; };

	AST_ShootTarget* SpawnTarget();

private:
	AST_TargetSpawningVolume* GetRandomSpawnVolume();

	FVector GetRandomSpawnLocation(const AST_TargetSpawningVolume* SpawnVolume);
};
