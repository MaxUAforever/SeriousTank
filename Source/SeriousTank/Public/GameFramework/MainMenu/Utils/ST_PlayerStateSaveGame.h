#pragma once

#include "GameFramework/SaveGame.h"
#include "ST_PlayerStateSaveGame.generated.h"

class AST_MainMenuPlayerState;
struct FVehicleInfo;

DECLARE_DELEGATE_OneParam(FPlayerStateAsyncSaveDelegate, bool);
DECLARE_DELEGATE_OneParam(FPlayerStateAsyncLoadDelegate, USaveGame*)

USTRUCT()
struct FPlayerStateSaveData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Basic")
    TArray<FVehicleInfo> AvailableVehicles;
	
	UPROPERTY(VisibleAnywhere, Category = "Basic")
    int32 CurrentVehicleIndex;
};

UCLASS()
class SERIOUSTANK_API UST_PlayerStateSaveGame : public USaveGame
{
	GENERATED_BODY()
    
private:
    static FString SaveSlotName;
    static uint32 UserIndex;
    
	UPROPERTY()
	FPlayerStateSaveData PlayerStateSaveData;
    
public:
    static void AsyncLoad(AST_MainMenuPlayerState* PlayerState, FPlayerStateAsyncLoadDelegate OnLoadedDelegate = FPlayerStateAsyncLoadDelegate());
    static void AsyncSave(const AST_MainMenuPlayerState* PlayerState, FPlayerStateAsyncSaveDelegate OnSavedDelegate = FPlayerStateAsyncSaveDelegate());

private:
	static void LoadDataFromSaveGame(AST_MainMenuPlayerState* PlayerState, USaveGame* LoadedGameData);
	static void StoreDataToSaveGame(UST_PlayerStateSaveGame* SaveGameInstance, const AST_MainMenuPlayerState* PlayerState);
};
