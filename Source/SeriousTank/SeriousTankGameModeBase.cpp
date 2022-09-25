#include "SeriousTankGameModeBase.h"

#include "Public/Actors/BaseTrackedVehicle.h"

void ASeriousTankGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	DefaultPawnClass = ABaseTrackedVehicle::StaticClass();
}