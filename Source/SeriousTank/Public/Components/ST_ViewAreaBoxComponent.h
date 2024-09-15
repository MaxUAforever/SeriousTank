#pragma once

#include "Components/BoxComponent.h"
#include "ST_ViewAreaBoxComponent.generated.h"

class UCameraComponent;

UCLASS()
class SERIOUSTANK_API UST_ViewAreaBoxComponent : public UBoxComponent
{
	GENERATED_BODY()
	
public:
	UST_ViewAreaBoxComponent();

protected:
	virtual void BeginPlay() override;

public:
	void UpdateViewArea();

private:
	UPROPERTY(EditAnywhere)
	float ViewAreaHight = 150.f;

	UPROPERTY(EditAnywhere)
	bool bDrawDebug = false;

	UCameraComponent* ParentCameraComponent;
};
