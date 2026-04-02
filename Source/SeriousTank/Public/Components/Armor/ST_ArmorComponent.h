#pragma once

#include "Components/ActorComponent.h"
#include "Components/Armor/ST_ArmorTypes.h"

#include "ST_ArmorComponent.generated.h"

class USceneComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SERIOUSTANK_API UST_ArmorComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UST_ArmorComponent();

    void SetArmorForComponent(USceneComponent* Component, const FST_ArmorSectionData& ArmorData);
    void RemoveArmorForComponent(USceneComponent* Component);
    const FST_ArmorSectionData* FindArmorForComponent(const USceneComponent* Component) const;

protected:
    virtual void BeginPlay() override;

private:
	// Rebuilds the ArmorSectionMap based on the current PredefinedArmorSections.
    void SetupArmorMap();

protected:
	// Predefined armor sections that can be set up in the editor.
    // These will be used to populate the ArmorSectionMap on BeginPlay.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Armor", meta = (TitleProperty = "SceneComponent"))
    TArray<FST_ArmorSectionDefinition> PredefinedArmorSections;

private:
    TMap<TWeakObjectPtr<const USceneComponent>, FST_ArmorSectionData> ArmorSectionMap;
};
