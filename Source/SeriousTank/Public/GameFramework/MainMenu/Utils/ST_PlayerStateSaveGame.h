#pragma once

#include "GameFramework/SaveGame.h"
#include "ST_PlayerStateSaveGame.generated.h"

class AST_MainMenuPlayerState;
struct FVehicleInfo;

DECLARE_DELEGATE_OneParam(FPlayerStateAsyncSaveDelegate, bool);

DECLARE_DELEGATE_OneParam(FPlayerStateAsyncLoadDelegate, USaveGame*)

UCLASS()
class SERIOUSTANK_API UST_PlayerStateSaveGame : public USaveGame
{
	GENERATED_BODY()
    
private:
    static FString SaveSlotName;
    static uint32 UserIndex;
    
    TArray<FVehicleInfo> AvailableVehicles;
    int32 CurrentVehicleIndex;
    
public:
    static void AsyncLoad(AST_MainMenuPlayerState* PlayerState, FPlayerStateAsyncLoadDelegate OnLoadedDelegate = FPlayerStateAsyncLoadDelegate());
    
    static void AsyncSave(const AST_MainMenuPlayerState* PlayerState, FPlayerStateAsyncSaveDelegate OnSavedDelegate = FPlayerStateAsyncSaveDelegate());
};
