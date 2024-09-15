#include "Core/Animation/ST_AnimNotify.h"

#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"

void UST_AnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp)
	{
		return;
	}

	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
	if (!AnimInstance)
	{
		return;
	}

	const FName FuncName("InternalAnimNotify_" + GetName());
	if (UFunction* Function = AnimInstance->FindFunction(FuncName))
	{
		AnimInstance->ProcessEvent(Function, nullptr);
	}
}