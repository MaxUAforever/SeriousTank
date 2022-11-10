#pragma once

#include "GameFramework/PlayerController.h"
#include "ST_GameplayPlayerController.generated.h"

UCLASS()
class SERIOUSTANK_API AST_GameplayPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void OnPauseGameClicked();

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

private:
	UFUNCTION()
	void SetPawnInputEnabled(bool IsEnabled);

	UFUNCTION()
	void SetOnlyUIInputEnabled(bool IsEnabled);
};
