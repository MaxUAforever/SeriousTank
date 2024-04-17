#include "Core/Animation/ST_AnimInstance.h"

#include "Core/Animation/ST_AnimMontageDataAsset.h"
#include "Core/Animation/ST_AnimNotify.h"

void UST_AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	ParseAnimationProperties(this);
}

void UST_AnimInstance::ParseAnimationProperties(UObject* Object)
{
	for (TFieldIterator<FObjectPropertyBase> PropertyIt(Object->GetClass()); PropertyIt; ++PropertyIt)
	{
		FObjectPropertyBase* Property = *PropertyIt;
		if (!Property)
		{
			continue;
		}

		void* ValueAddress = Property->ContainerPtrToValuePtr<void>(Object);
		UObject* PropertyObject = Property->GetObjectPropertyValue(ValueAddress);
		if (!PropertyObject)
		{
			continue;
		}

		if (UST_AnimMontageDataAsset* MontagesDataAsset = Cast<UST_AnimMontageDataAsset>(PropertyObject))
		{
			ParseAnimationProperties(MontagesDataAsset);
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
