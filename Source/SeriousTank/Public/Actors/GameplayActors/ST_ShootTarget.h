#pragma once

#include "GameFramework/Actor.h"
#include "ST_ShootTarget.generated.h"

class UBoxComponent;

UCLASS()
class SERIOUSTANK_API AST_ShootTarget : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere)
	UBoxComponent* CollisionCompnent;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComponent;

public:	
	AST_ShootTarget();
};
