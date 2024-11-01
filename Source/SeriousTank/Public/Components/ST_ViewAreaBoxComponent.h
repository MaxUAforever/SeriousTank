#pragma once

#include "Components/BoxComponent.h"
#include "ST_ViewAreaBoxComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnFrontViewDistanceChanged, float);

class UCameraComponent;

UCLASS()
class SERIOUSTANK_API UST_ViewAreaBoxComponent : public UBoxComponent
{
	GENERATED_BODY()
	
public:
	FOnFrontViewDistanceChanged OnFrontViewDistanceChanged;

public:
	UST_ViewAreaBoxComponent();

protected:
	virtual void BeginPlay() override;

public:
	void UpdateViewArea();

	FORCEINLINE float GetFrontViewDistance() const { return FrontViewDistance; }

private:
	UPROPERTY(EditAnywhere)
	float ViewAreaHight = 150.f;

	UPROPERTY(EditAnywhere)
	bool bDrawDebug = false;

	float FrontViewDistance;

	UCameraComponent* ParentCameraComponent;
};
