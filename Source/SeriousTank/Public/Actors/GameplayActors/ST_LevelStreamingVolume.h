#pragma once

#include "GameFramework/Actor.h"
#include "ST_LevelStreamingVolume.generated.h"

class UBoxComponent;

UCLASS()
class SERIOUSTANK_API AST_LevelStreamingVolume : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* OverlapVolume;

	UPROPERTY(EditAnywhere)
	FName LevelToLoad;

public:
	AST_LevelStreamingVolume();

protected:
	UFUNCTION()
	void OverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OverlapEnds(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
