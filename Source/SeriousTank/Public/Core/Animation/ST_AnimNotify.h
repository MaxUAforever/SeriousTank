#pragma once

#include "Animation/AnimNotifies/AnimNotify.h"
#include "ST_AnimNotify.generated.h"

UCLASS()
class SERIOUSTANK_API UST_AnimNotify : public UAnimNotify
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AnimNotify")
	FString Name;

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	const FString& GetName() { return Name; }
};
