#pragma once

#include "GameFramework/GameModeBase.h"
#include "ST_GameplayGameMode.generated.h"

class AST_ShootTarget;
class UST_TargetRespawnManager;

USTRUCT(BlueprintType)
struct FGameData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, Category="Game", meta = (ClampMin = "0", ClampMax = "5"))
	int32 PreStartCountdownTime = 3;

	UPROPERTY(EditDefaultsOnly, Category="Game", meta = (ClampMin = "1", ClampMax = "500"))
	int32 StartTime = 50;

	UPROPERTY(EditDefaultsOnly, Category="Game", meta = (ClampMin = "1", ClampMax = "500"))
	float AddedTimeForTarget = 5.f;

	UPROPERTY(EditDefaultsOnly, Category="Game", meta = (ClampMin = "1", ClampMax = "500"))
	int32 AddedScoreForTarget = 1;

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

public:
	UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

protected:
	virtual void BeginPlay() override;

	virtual void InitGameState() override;

private:
	UFUNCTION()
	void OnTargetDestroyed(AActor* DestroyedActor);

	UFUNCTION()
	void OnGameOver();

	void SpawnTarget();
};