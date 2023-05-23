#pragma once

#include "GameFramework/Gameplay/ST_GameplayGameMode.h"
#include "ST_TargetShootingGameMode.generated.h"

class UST_TargetRespawnManager;

UCLASS()
class SERIOUSTANK_API AST_TargetShootingGameMode : public AST_GameplayGameMode
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Game")
	FTargetShootingGameData GameData;

protected:
	virtual void BeginPlay() override;
	virtual void InitGameState() override;

private:
	UFUNCTION()
	void OnTargetDestroyed(AActor* DestroyedActor);
	void OnTargetSpawned(AActor* InTargetActor);
};
