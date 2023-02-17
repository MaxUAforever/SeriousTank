#pragma once

#include "GameFramework/ST_BasePlayerController.h"
#include "ST_MainMenuPlayerController.generated.h"

UCLASS()
class SERIOUSTANK_API AST_MainMenuPlayerController : public AST_BasePlayerController
{
	GENERATED_BODY()
	
public:
	void OnNewGameStarted();

protected:
	virtual void BeginPlay() override;
};
