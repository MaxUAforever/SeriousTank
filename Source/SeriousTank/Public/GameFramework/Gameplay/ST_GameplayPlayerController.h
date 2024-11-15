#pragma once

#include "GameFramework/PlayerController.h"
#include "ST_GameplayPlayerController.generated.h"

class UInputAction;
class UInputMappingContext;
enum class EHealthChangingType : uint8;

DECLARE_MULTICAST_DELEGATE(FOnMainCharacterDiedDelegate);

UCLASS()
class SERIOUSTANK_API AST_GameplayPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	FOnMainCharacterDiedDelegate OnMainCharacterDiedDelegate;

protected:
	UPROPERTY(Category = "Input",  EditDefaultsOnly)
    UInputMappingContext* CommonGameplayInputContext;

	UPROPERTY(Category = "Input",  EditDefaultsOnly)
    UInputAction* PauseInputAction;

private:
	APawn* PreviousPawn;
	APawn* MainGameplayPawn;

	FDelegateHandle HealthChangeDelegateHandle;

public:
	UFUNCTION()
	void OnPauseGameClicked();

	APawn* GetPreviousPawn() const { return PreviousPawn; }

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

private:
	UFUNCTION()
	void SetPawnInputEnabled(bool IsEnabled);

	UFUNCTION()
	void SetOnlyUIInputEnabled(bool IsEnabled);

	void SetupHealthComponent(APawn* InPawn);
	void OnHealthChanged(float CurrentHealthValue, EHealthChangingType HealthChangingType);
};
