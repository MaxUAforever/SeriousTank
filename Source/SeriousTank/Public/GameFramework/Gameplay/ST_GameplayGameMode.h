#pragma once

#include "Core/ST_CoreTypes.h"
#include "GameFramework/GameModeBase.h"
#include "ST_GameplayGameMode.generated.h"

struct FObjectTypeSpawnParams;

UCLASS()
class SERIOUSTANK_API AST_GameplayGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FOnGamePaused, bool)
	FOnGamePaused OnGamePaused;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Game")
	FBaseGameData BaseGameData;

	UPROPERTY(EditDefaultsOnly, Category = "Game")
	TArray<FObjectTypeSpawnParams> ObjectsSpawnParameters;

public:
    AST_GameplayGameMode();
    
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

public:
	virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate = FCanUnpause()) override;
	virtual bool ClearPause() override;

protected:
	virtual void BeginPlay() override;

	virtual void InitGameState() override;

	virtual void OnPreStartCountdownEneded() {};

	void TriggerGameFinish();

private:
	void SetupDeathHandling();
	void SetupSpawnSubsystem();
};
