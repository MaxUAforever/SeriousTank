#pragma once

#include "GameFramework/PlayerController.h"
#include "ST_BasePlayerController.generated.h"

UCLASS()
class SERIOUSTANK_API AST_BasePlayerController : public APlayerController
{
	GENERATED_BODY()

public: 
	DECLARE_MULTICAST_DELEGATE(FOnNavigationKeyPressed)
	FOnNavigationKeyPressed OnNavigationKeyPressed;

protected:
	virtual void SetupInputComponent() override;

private:
	UFUNCTION()
	void HandleNavigationKeyPressed();
};
