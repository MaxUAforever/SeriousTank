#pragma once

#include "GameFramework/PlayerController.h"
#include "ST_MainMenuPlayerController.generated.h"

UCLASS()
class SERIOUSTANK_API AST_MainMenuPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AST_MainMenuPlayerController();

	void OnNewGameStarted();

protected:
	virtual void BeginPlay() override;
};
