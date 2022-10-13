#pragma once

#include "GameFramework/Actor.h"
#include "ST_TargetSpawningVolume.generated.h"

class UBoxComponent;

UCLASS()
class SERIOUSTANK_API AST_TargetSpawningVolume : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere)
	UBoxComponent* SpawningBoxComponent;

public:	
	AST_TargetSpawningVolume();
};
