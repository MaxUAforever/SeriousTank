#pragma once

#include "Core/ST_CoreTypes.h"
#include "GameFramework/GameModeBase.h"
#include "ST_GameplayGameMode.generated.h"

class UST_TargetRespawnManager;

UCLASS()
class SERIOUSTANK_API AST_GameplayGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FOnGamePaused, bool)
	FOnGamePaused OnGamePaused;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Game")
	FGameData GameData;
	
private:
	UPROPERTY()
	UST_TargetRespawnManager* TargetRespawnManager;

public:
    AST_GameplayGameMode();
    
	UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

public:
	virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate = FCanUnpause()) override;

	virtual bool ClearPause() override;

protected:
	virtual void BeginPlay() override;

	virtual void InitGameState() override;

private:
	UFUNCTION()
	void OnTargetDestroyed(AActor* DestroyedActor);

	void SpawnTarget();
};
