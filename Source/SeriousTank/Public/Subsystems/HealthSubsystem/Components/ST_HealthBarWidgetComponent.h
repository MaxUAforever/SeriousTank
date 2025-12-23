#pragma once

#include "Components/WidgetComponent.h"
#include "ST_HealthBarWidgetComponent.generated.h"

class UST_HealthUserWidget;
enum class EHealthChangingType : uint8;
enum class ETeamRelationType : uint8;

/**
 * Component that allows to control widget for Health bar above player.
 */
UCLASS()
class SERIOUSTANK_API UST_HealthBarWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	void SetupColor(ETeamRelationType TeamRelationType);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	void OnHealthValueChanged(float NewHealthValue, EHealthChangingType HealthChangingType);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	float HeightOffset = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FLinearColor AllyHealthBarColor = FLinearColor::Green;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FLinearColor EnemyHealthBarColor = FLinearColor::Red;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FLinearColor NeutralHealthBarColor = FLinearColor::Gray;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TSubclassOf<UST_HealthUserWidget> HealthBarWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	bool bIsHiddenForPlayer = true;

private:
	TObjectPtr<UST_HealthUserWidget> HealthBarWidget;
};
