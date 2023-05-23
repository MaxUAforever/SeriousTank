// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Subsystems/WorldSubsystem.h"

#include "Subsystems/ObjectSpawnSubsystem/ObjectSpawnManager.h"
#include "Subsystems/ObjectSpawnSubsystem/SpawnSubsystemTypes.h"
#include "ObjectSpawnSubsystem.generated.h"

UCLASS()
class SERIOUSTANK_API UObjectSpawnSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	TMap<ESpawnObjectType, UObjectSpawnManager*> ObjectSpawnManagers;

public:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	UObjectSpawnManager* AddObjectSpawnManager(ESpawnObjectType SpawnObjectType, const FObjectSpawnParameters& SpawnParameters);
	UObjectSpawnManager* GetObjectSpawnManager(ESpawnObjectType SpawnObjectType);
};
