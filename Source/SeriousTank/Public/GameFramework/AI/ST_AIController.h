#pragma once

#include "AIController.h"
#include "ST_AIController.generated.h"

class AST_BaseWeapon;
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
	
protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

private:
	void SetupPerception(APawn* InPawn);
	void SetupHealthSubsystem(APawn* InPawn);
	void SetupWeaponsComponent(APawn* InPawn);

protected:
	UPROPERTY(EditDefaultsOnly)
	UBehaviorTree* DefaultBehaviourTree;

	UPROPERTY(EditDefaultsOnly)
	EViewPerceptionType ViewPerceptionType = EViewPerceptionType::PlayerView;

	UPROPERTY(EditDefaultsOnly)
	FName BBSightPerceptionTypeKey = FName("SightPerseptionType");

	UPROPERTY(EditAnywhere)
	FName BBAttackTargetKey = FName("AttackTarget");

	UPROPERTY(EditAnywhere, Category = Blackboard)
	FName BBCanAttackKey = FName("CanAttack");

private:
	UFUNCTION()
	void OnTargetOverlapViewBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnTargetEndOverlapViewBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void OnWeaponAdded(int32 WeaponIndex, AST_BaseWeapon* Weapon);
	
	void OnTargetDetected(AActor* Target);
	void OnLostTarget(AActor* OtherActor);

	void OnHealthChanged(float CurrentHealthValue, EHealthChangingType HealthChangingType);
};
