#include "GameFramework/MainMenu/Utils/ST_PlayerStateSaveGame.h"

#include "GameFramework/MainMenu/ST_MainMenuPlayerState.h"

#include "Kismet/GameplayStatics.h"

FString UST_PlayerStateSaveGame::SaveSlotName = TEXT("MainPlayerStateSlot");
uint32 UST_PlayerStateSaveGame::UserIndex = 0;

void UST_PlayerStateSaveGame::AsyncLoad(AST_MainMenuPlayerState* PlayerState, FPlayerStateAsyncLoadDelegate OnLoadedDelegate)
{
    auto OnAsyncLoadDone = [PlayerState, OnLoadedDelegate](const FString& /*SlotName*/, const int32 /*UserIndex*/, USaveGame* LoadedGameData)
    {
		LoadDataFromSaveGame(PlayerState, LoadedGameData);
        OnLoadedDelegate.ExecuteIfBound(LoadedGameData);
    };
    
    if (!UGameplayStatics::DoesSaveGameExist(SaveSlotName, UserIndex))
    {
        OnLoadedDelegate.ExecuteIfBound(nullptr);
        return;
    }
    
    FAsyncLoadGameFromSlotDelegate LoadedDelegate = FAsyncLoadGameFromSlotDelegate::CreateLambda(OnAsyncLoadDone);
    UGameplayStatics::AsyncLoadGameFromSlot(SaveSlotName, UserIndex, LoadedDelegate);
}

void UST_PlayerStateSaveGame::AsyncSave(const AST_MainMenuPlayerState* PlayerState, FPlayerStateAsyncSaveDelegate OnSavedDelegate)
{
    USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(ThisClass::StaticClass());
    UST_PlayerStateSaveGame* SaveGameInstance = Cast<UST_PlayerStateSaveGame>(SaveGameObject);
    if (!SaveGameInstance || !PlayerState)
    {
        OnSavedDelegate.ExecuteIfBound(false);
    }
	
	StoreDataToSaveGame(SaveGameInstance, PlayerState);
    
    auto OnAsyncSaveDone = [OnSavedDelegate](const FString& /*SlotName*/, const int32 /*UserIndex*/, bool bSuccess)
    {
        OnSavedDelegate.ExecuteIfBound(bSuccess);
    };
    
    FAsyncSaveGameToSlotDelegate SavedDelegate = FAsyncSaveGameToSlotDelegate::CreateLambda(OnAsyncSaveDone);
    UGameplayStatics::AsyncSaveGameToSlot(SaveGameInstance, SaveSlotName, UserIndex, SavedDelegate);
}

void UST_PlayerStateSaveGame::LoadDataFromSaveGame(AST_MainMenuPlayerState* PlayerState, USaveGame* LoadedGameData)
{
	UST_PlayerStateSaveGame* LoadedPlayerStateData = Cast<UST_PlayerStateSaveGame>(LoadedGameData);
	if (!PlayerState || !LoadedPlayerStateData)
	{
		return;
	}
		
	const FPlayerStateSaveData& PlayerStateSaveData = LoadedPlayerStateData->PlayerStateSaveData;
	if (PlayerStateSaveData.AvailableVehicles.Num() > 0)
	{
		PlayerState->SetVehicles(PlayerStateSaveData.AvailableVehicles);
	}
	if (PlayerStateSaveData.CurrentVehicleIndex >= 0 && PlayerStateSaveData.CurrentVehicleIndex < PlayerStateSaveData.AvailableVehicles.Num())
	{
		PlayerState->SetCurrentVehicle(PlayerStateSaveData.CurrentVehicleIndex);
	}
}

void UST_PlayerStateSaveGame::StoreDataToSaveGame(UST_PlayerStateSaveGame* SaveGameInstance, const AST_MainMenuPlayerState* PlayerState)
{
	if (SaveGameInstance && PlayerState)
	{
		SaveGameInstance->PlayerStateSaveData.AvailableVehicles = PlayerState->GetVehicles();
		SaveGameInstance->PlayerStateSaveData.CurrentVehicleIndex = PlayerState->GetCurrentVehicleIndex();
	}
}