#include "Subsystems/HealthSubsystem/Components/ST_ArmorComponent.h"

#include "Components/SceneComponent.h"
#include "Algo/Find.h"

UST_ArmorComponent::UST_ArmorComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UST_ArmorComponent::BeginPlay()
{
    Super::BeginPlay();

    SetupArmorMap();
}

void UST_ArmorComponent::SetArmorForComponent(USceneComponent* Component, const FST_ArmorSectionData& ArmorData)
{
    if (!IsValid(Component))
    {
		UE_LOG(LogArmorComponent, Warning, TEXT("%s: Cannot set armor for invalid component."), ANSI_TO_TCHAR(__FUNCTION__));
        return;
    }

    ArmorSectionMap.Add(Component, ArmorData);
}

void UST_ArmorComponent::RemoveArmorForComponent(USceneComponent* Component)
{
    if (!IsValid(Component))
    {
        UE_LOG(LogArmorComponent, Warning, TEXT("%s: Cannot remove armor for invalid component."), ANSI_TO_TCHAR(__FUNCTION__));
        return;
    }

    ArmorSectionMap.Remove(Component);
}

const FST_ArmorSectionData* UST_ArmorComponent::FindArmorForComponent(const USceneComponent* Component) const
{
    if (!IsValid(Component))
    {
        return nullptr;
    }

    return ArmorSectionMap.Find(Component);
}

void UST_ArmorComponent::SetupArmorMap()
{
    ArmorSectionMap.Reset();
	ArmorSectionMap.Reserve(PredefinedArmorSections.Num());

    for (const FST_ArmorSectionDefinition& Definition : PredefinedArmorSections)
    {
        const USceneComponent* Component = Cast<USceneComponent>(Definition.SceneComponentRef.GetComponent(GetOwner()));
        if (!IsValid(Component))
        {
			UE_LOG(LogArmorComponent, Warning, TEXT("%s: Skipping invalid SceneComponent in PredefinedArmorSections."), ANSI_TO_TCHAR(__FUNCTION__));
            continue;
        }

        ArmorSectionMap.Add(Component, Definition.ArmorData);
    }
}
