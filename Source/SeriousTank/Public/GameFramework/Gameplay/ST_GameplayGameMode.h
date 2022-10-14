#pragma once

#include "GameFramework/GameModeBase.h"
#include "ST_GameplayGameMode.generated.h"

class AST_ShootTarget;
class UST_TargetRespawnManager;

USTRUCT(BlueprintType)
struct FGameData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, Category="Game", meta = (ClampMin = "1", ClampMax = "500"))
	int32 StartTime = 50;

	UPROPERTY(EditDefaultsOnly, Category="Game", meta = (ClampMin = "1", ClampMax = "500"))
	int32 AddedTimeForTarget = 5;

	UPROPERTY(EditDefaultsOnly, Category = "Game")
	TSubclassOf<AST_ShootTarget> ShootingTargetClass;
};

UCLASS()
class SERIOUSTANK_API AST_GameplayGameMode : public AGameModeBase
{
	GENERATED_BODY()
		
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Game")
	FGameData GameData;
	
private:
	UPROPERTY()
	UST_TargetRespawnManager* TargetRespawnManager;

protected:
	virtual void BeginPlay() override;

	virtual void InitGameState() override;

private:
	UFUNCTION()
	void OnTargetDestroyed(AActor* DestroyedActor);

	void SpawnTarget();
};