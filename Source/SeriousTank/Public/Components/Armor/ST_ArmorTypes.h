#pragma once

#include "Components/ActorComponent.h"

#include "ST_ArmorTypes.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogArmorComponent, Log, All);

class USceneComponent;

USTRUCT(BlueprintType)
struct FST_ArmorSectionData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Armor")
    float ArmorValue = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Armor")
    bool bAffectsByHitAngle = true;
};

USTRUCT(BlueprintType)
struct FST_ArmorSectionDefinition
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Armor")
    FComponentReference SceneComponentRef;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Armor")
    FST_ArmorSectionData ArmorData;
};