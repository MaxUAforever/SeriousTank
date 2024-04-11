#pragma once

#include "Components/SceneComponent.h"
#include "ST_LeftHandAttachComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SERIOUSTANK_API UST_LeftHandAttachComponent : public USceneComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;	
};
