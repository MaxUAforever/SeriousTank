#include "Core/Animation/ST_AnimInstance.h"

#include "Core/Animation/ST_AnimNotify.h"

void UST_AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	for (TFieldIterator<FObjectPropertyBase> PropertyIt(this->GetClass()); PropertyIt; ++PropertyIt)
	{
		FObjectPropertyBase* Property = *PropertyIt;
		if (!Property || !Property->GetCPPType().Contains("UAnimMontage"))
		{
			continue;
		}

		void* ValueAddress = Property->ContainerPtrToValuePtr<void>(this);
		UObject* PropertyObject = Property->GetObjectPropertyValue(ValueAddress);
		if (!PropertyObject)
		{
			continue;
		}

		UAnimMontage* AnimMontage = Cast<UAnimMontage>(PropertyObject);
		if (!AnimMontage)
		{
			continue;
		}

		for (FAnimNotifyEvent& AnimNotifyEvent : AnimMontage->Notifies)
		{
			UST_AnimNotify* AnimNotify = Cast<UST_AnimNotify>(AnimNotifyEvent.Notify);
			if (!AnimNotify)
			{
				continue;
			}

			const FName FuncName("InternalAnimNotify_" + AnimNotify->GetName());
			if (this->FindFunction(FuncName))
			{
				AnimNotify->OnAnimNotifyTriggered.BindUFunction(this, FuncName);
			}
		}
	}
}