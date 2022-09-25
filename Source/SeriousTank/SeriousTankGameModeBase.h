// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SeriousTankGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class SERIOUSTANK_API ASeriousTankGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
};
