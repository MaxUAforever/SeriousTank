#pragma once

#include "GameFramework/PlayerController.h"
#include "ST_GameplayPlayerController.generated.h"

UCLASS()
class SERIOUSTANK_API AST_GameplayPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void SetInputEnabled(bool IsEnabled);
};
