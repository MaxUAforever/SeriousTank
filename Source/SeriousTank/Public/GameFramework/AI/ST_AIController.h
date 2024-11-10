#pragma once

#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "ST_AIController.generated.h"

class AST_BaseWeapon;
class UAIPatrollingComponent;
class UAIPerceptionComponent;
enum class EHealthChangingType : uint8;

UENUM(BlueprintType)
enum class EViewPerceptionType : uint8
{
	PlayerView,
	AIPerception
};

UCLASS()
class SERIOUSTANK_API AST_AIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AST_AIController();

protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

private:
	void SetupPerception(APawn* InPawn);
	void SetupHealthSubsystem(APawn* InPawn);
	void SetupWeaponsComponent(APawn* InPawn);

	UFUNCTION()
	void OnTargetOverlapViewBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnTargetEndOverlapViewBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnSensePerceptionTriggered(AActor* Actor, FAIStimulus Stimulus);

	void ChangeSightRadius(float InSightRadius);

	void OnPatrollingStateChaned(bool bIsActive);

	void OnWeaponAdded(int32 WeaponIndex, AST_BaseWeapon* Weapon);
	void OnTargetDetected(AActor* Target);
	void OnTargetLost(AActor* OtherActor);
	void OnHealthChanged(float CurrentHealthValue, EHealthChangingType HealthChangingType);

protected:
	UPROPERTY(EditDefaultsOnly)
	UBehaviorTree* DefaultBehaviourTree;

	UPROPERTY(EditDefaultsOnly)
	UAIPerceptionComponent* PerceptionComp;

	UPROPERTY(EditDefaultsOnly)
	UAIPatrollingComponent* PatrollingComponent;

	UPROPERTY(EditDefaultsOnly)
	EViewPerceptionType ViewPerceptionType = EViewPerceptionType::PlayerView;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "ViewPerceptionType==EViewPerceptionType::AIPerception", ClampMin = "0.0"))
	float PerceptionSightRadiusScale = 1.f;

	/**
	 * Blackboard keys 
	 */
	UPROPERTY(EditDefaultsOnly)
	FName BBSightPerceptionTypeKey = FName("SightPerseptionType");

	UPROPERTY(EditAnywhere)
	FName BBAttackTargetKey = FName("AttackTarget");

	UPROPERTY(EditAnywhere, Category = Blackboard)
	FName BBCanAttackKey = FName("CanAttack");

	UPROPERTY(EditAnywhere, Category = Blackboard)
	FName BBCanPatrol = FName("CanPatrol");
};
