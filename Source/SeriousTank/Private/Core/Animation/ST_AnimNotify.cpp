#include "Core/Animation/ST_AnimNotify.h"

void UST_AnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	OnAnimNotifyTriggered.ExecuteIfBound();
}