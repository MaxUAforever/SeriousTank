#pragma once

#include "UObject/Object.h"
#include "ST_TargetRespawnManager.generated.h"

class AST_ShootTarget;
class AST_TargetSpawningVolume;

UCLASS(BlueprintType)
class SERIOUSTANK_API UST_TargetRespawnManager : public UObject
{
	GENERATED_BODY()

private:
	TArray<AST_TargetSpawningVolume*> SpawnVolumes;

	float SpawningHeight = 50.f;

public:
	UST_TargetRespawnManager();

	AST_ShootTarget* SpawnTarget(TSubclassOf<AST_ShootTarget> ShootTargetClass);

private:
	AST_TargetSpawningVolume* GetRandomSpawnVolume();

	FVector GetRandomSpawnLocation(const AST_TargetSpawningVolume* SpawnVolume);
};
