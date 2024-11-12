#pragma once

#include "Components/ActorComponent.h"
#include "AIPatrollingComponent.generated.h"

class AAIPatrolPath;
struct FPatrolPathPointInfo;
enum class EDirectionType : uint8;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnIsActiveChanged, bool);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AIPATROLLINGSUBSYSTEM_API UAIPatrollingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FOnIsActiveChanged OnIsActiveChanged;

protected:
	virtual void BeginPlay() override;

public:
	FORCEINLINE bool IsPatrollingActive() const { return bIsPatrollingActive; }
	void SetIsPatrollingActive(bool bInIsActive);

	const AAIPatrolPath* GetCurrentPatrolPath() const { return CurrentPatrolPath; }
	int32 GetCurrentPathPointIndex() const { return CurrentPathPointIndex; }
	EDirectionType GetCurrentDirection() const { return CurrentDirection; }

	void SetPatrolPath(const AAIPatrolPath* InPatrolPath) { CurrentPatrolPath = InPatrolPath; }
	void SetCurrentPathPointIndex(int32 InPathPointIndex) { CurrentPathPointIndex = InPathPointIndex; }
	void SetCurrentDirection(EDirectionType InDirection) { CurrentDirection = InDirection; }

protected:
	UPROPERTY(EditAnywhere)
	bool bStartWithPatrollingActive = true;

private:
	const AAIPatrolPath* CurrentPatrolPath;
	int32 CurrentPathPointIndex;
	EDirectionType CurrentDirection;

	bool bIsPatrollingActive = false;
};
