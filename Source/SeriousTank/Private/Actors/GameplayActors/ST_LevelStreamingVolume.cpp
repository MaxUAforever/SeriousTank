#include "Actors/GameplayActors/ST_LevelStreamingVolume.h"

#include "Components/BoxComponent.h"
#include "Components/ST_ViewAreaBoxComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

AST_LevelStreamingVolume::AST_LevelStreamingVolume()
{
	OverlapVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapVolume"));
	RootComponent = OverlapVolume;

	OverlapVolume->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OverlapBegins);
	OverlapVolume->OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::OverlapEnds);
}

void AST_LevelStreamingVolume::OverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UWorld* World = GetWorld();
	if (OtherComp->IsA(UST_ViewAreaBoxComponent::StaticClass()) && !LevelToLoad.IsNone() && World != nullptr)
	{
		FLatentActionInfo LatentInfo;
		UGameplayStatics::LoadStreamLevel(World, LevelToLoad, true, true, LatentInfo);
	}
}

void AST_LevelStreamingVolume::OverlapEnds(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UWorld* World = GetWorld();
	if (OtherComp->IsA(UST_ViewAreaBoxComponent::StaticClass()) && !LevelToLoad.IsNone() && World != nullptr)
	{
		FLatentActionInfo LatentInfo;
		UGameplayStatics::UnloadStreamLevel(World, LevelToLoad, LatentInfo, false);
	}
}