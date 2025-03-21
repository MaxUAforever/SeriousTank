#pragma once

#include "UI/ViewModel/ST_ViewModelBase.h"
#include "ST_VMHealth.generated.h"

class APlayerController;
enum class EHealthChangingType : uint8;

DECLARE_DELEGATE_OneParam(FOnHealthViewChangedDelegate, float);
DECLARE_DELEGATE_OneParam(FOnMaxHealthViewChangedDelegate, float);

UCLASS()
class SERIOUSTANK_API UST_VMHealth : public UST_ViewModelBase
{
	GENERATED_BODY()
	
public:
	FOnHealthViewChangedDelegate OnHealthChangedDelegate;
	FOnMaxHealthViewChangedDelegate OnMaxHealthChangedDelegate;

public:
	virtual void Initialize() override;

private:
	void OnHealthChanged(float CurrentHealthValue, EHealthChangingType HealthChangingType) const;
	void OnMaxHealthChanged(float MaxHealthValue) const;

	UFUNCTION()
	void OnPawnChanged(APawn* OldPawn, APawn* NewPawn);

private:
	FDelegateHandle OnHealthChangedDelegateHandle;
	FDelegateHandle OnMaxHealthChangedDelegateHandle;
};
