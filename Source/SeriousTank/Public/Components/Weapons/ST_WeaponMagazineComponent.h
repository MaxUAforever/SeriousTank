#pragma once

#include "Components/StaticMeshComponent.h"
#include "ST_WeaponMagazineComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SERIOUSTANK_API UST_WeaponMagazineComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;		
};
