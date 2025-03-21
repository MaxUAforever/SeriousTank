// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Subsystems/ObjectSpawnSubsystem/BaseObjectSpawner.h"
#include "ObjectSpawnVolume.generated.h"

class UBoxComponent;

UCLASS()
class SERIOUSTANK_API AObjectSpawnVolume : public ABaseObjectSpawner
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	UBoxComponent* SpawningBoxComponent;
	
public:
	AObjectSpawnVolume();

private:
	virtual FVector GetSpawnLocation() const override;

};
