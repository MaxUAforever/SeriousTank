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

UENUM(BlueprintType)
enum class EEnemyType : uint8
{
	Player,
	Team
};

UCLASS()
class SERIOUSTANK_API AST_AIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AST_AIController();

	inline void SetEnemyType(EEnemyType NewEnemyType) { EnemyType = NewEnemyType; };

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type Reason) override;

	virtual void OnPossess(APawn* InPawn) override;

private:
	void SetupPawnSettings();

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
	void OnAmmoRefilled(int32 WeaponIndex, AST_BaseWeapon* Weapon);
	void OnWeaponOutOfAmmo(int32 WeaponIndex, AST_BaseWeapon* Weapon);
	void OnWeaponSwitched(int32 PreviousWeaponIndex, int32 NewWeaponIndex);

	void OnTargetDetected(AActor* Target);
	void OnTargetLost(AActor* OtherActor);
	void UpdateTargetDetection();

	void OnHealthChanged(float CurrentHealthValue, EHealthChangingType HealthChangingType);
	void OnAttackTargetHealthChanged(float CurrentHealthValue, EHealthChangingType HealthChangingType);

	void OnAttackTargetChanged(AActor* Target);
	void AimToTarget();

	UFUNCTION()
	void OnTargetVehicleTaken(APawn* InPawn, AController* OldController, AController* NewController);

protected:
	UPROPERTY(EditDefaultsOnly)
	UBehaviorTree* DefaultBehaviourTree;

	UPROPERTY(EditDefaultsOnly)
	UBehaviorTree* TankBehaviourTree;

	UPROPERTY(EditDefaultsOnly)
	UAIPerceptionComponent* PerceptionComp;

	UPROPERTY(EditDefaultsOnly)
	UAIPatrollingComponent* PatrollingComponent;

	UPROPERTY(EditDefaultsOnly)
	EViewPerceptionType ViewPerceptionType = EViewPerceptionType::PlayerView;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "ViewPerceptionType==EViewPerceptionType::AIPerception", ClampMin = "0.0"))
	float PerceptionSightRadiusScale = 1.f;

	UPROPERTY(EditAnywhere)
	EEnemyType EnemyType = EEnemyType::Player;

	UPROPERTY(EditAnywhere)
	bool bCanPossessVehicles = true;

	bool bIsInVehicle = false;

	/**
	 * Blackboard keys 
	 */
	UPROPERTY(EditDefaultsOnly)
	FName BBSightPerceptionTypeKey = FName("SightPerseptionType");

	UPROPERTY(EditAnywhere)
	FName BBAttackTargetKey = FName("AttackTarget");

	UPROPERTY(EditAnywhere)
	FName BBFreeTrackedVehicleKey = FName("FreeTrackedVehicle");

	UPROPERTY(EditAnywhere, Category = Blackboard)
	FName BBHasWeaponKey = FName("HasWeapon");

	UPROPERTY(EditAnywhere, Category = Blackboard)
	FName BBWeaponSwitchNeededKey = FName("WeaponSwitchNeeded");

	UPROPERTY(EditAnywhere, Category = Blackboard)
	FName BBCanAttackKey = FName("CanAttack");

	UPROPERTY(EditAnywhere, Category = Blackboard)
	FName BBCanPatrol = FName("CanPatrol");

	UPROPERTY(EditAnywhere, Category = Blackboard)
	FName BBIsAimingKey = FName("IsAiming");

private:
	TObjectPtr<const AActor> AttackTarget;
	FTimerHandle AimUpdateTimerHandle;
};
