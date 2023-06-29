#pragma once

#include "Components/ActorComponent.h"
#include "ST_WeaponSocketManagerComponent.generated.h"

class UST_WeaponSocketComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SERIOUSTANK_API UST_WeaponSocketManagerComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	TArray<UST_WeaponSocketComponent*> WeaponSockets;

protected:
	virtual void BeginPlay() override;	

public:
	const TArray<UST_WeaponSocketComponent*>& GetWeaponSockets() const { return WeaponSockets; }
	UST_WeaponSocketComponent* GetWeaponSocket(int32 Index) const;
};
