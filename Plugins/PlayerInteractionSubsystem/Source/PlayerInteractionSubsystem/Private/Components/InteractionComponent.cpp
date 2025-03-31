#include "Components/InteractionComponent.h"

#include "Actions/BaseInteractionAction.h"
#include "Components/WidgetComponent.h"
#include "Components/InteractingComponent.h"
#include "Components/InteractionWidgetComponent.h"
#include "PlayerInteractionCoreTypes.h"
#include "PlayerInteractionSubsystem.h"
#include "UI/InteractionUserWidget.h"

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!InteractionActionClass)
	{
		UE_LOG(LogPlayerInteractionSubsystem, Warning, TEXT("InteractionComponent::BeginPlay: InteractionActionClass isn't set"));
		return;
	}
	
	OnComponentBeginOverlap.AddDynamic(this, &ThisClass::HandleBeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &ThisClass::HandleEndOverlap);

	PlayerInteractionSubsystem = GetWorld() ? GetWorld()->GetSubsystem<UPlayerInteractionSubsystem>() : nullptr;

	if (bIsWidgetEnabled && InteractionWidgetClass)
	{
		InteractionWidgetComponent = NewObject<UInteractionWidgetComponent>(GetOwner(), UInteractionWidgetComponent::StaticClass(), TEXT("InteractionWidget"));
		if (InteractionWidgetComponent)
		{
			InteractionWidgetComponent->RegisterComponent();
			InteractionWidgetComponent->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			
			InteractionWidgetComponent->SetVisibility(false);

			InteractionWidgetComponent->SetWidgetClass(InteractionWidgetClass);
			InteractionWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
			InteractionWidgetComponent->SetRelativeLocation(FVector{ 0.f, 0.f, -40.f });
			InteractionWidgetComponent->UpdateWidgetData();
		}
	}

	TArray<USceneComponent*> ChildComponents;
	GetChildrenComponents(false, ChildComponents);
}

void UInteractionComponent::SetIsInteractionComponentActive(bool bInIsActive)
{
	if (bIsActive == bInIsActive || !PlayerInteractionSubsystem)
	{
		return;
	}

	if (bInIsActive)
	{
		TSet<AActor*> OverlappingActors;
		GetOverlappingActors(OverlappingActors);

		for (AActor* OverlappingActor : OverlappingActors)
		{
			if (UInteractingComponent* InteractingComponent = OverlappingActor->GetComponentByClass<UInteractingComponent>())
			{
				PlayerInteractionSubsystem->RegisterInteraction(InteractingComponent, this);
			}
		}
	}
	else
	{
		PlayerInteractionSubsystem->RemoveInteraction(this);
	}

	bIsActive = bInIsActive;

	if (InteractionWidgetComponent)
	{
		InteractionWidgetComponent->SetVisibility(bIsActive);
	}
}

TArray<FTransform> UInteractionComponent::GetWorldInteractionPoints() const
{
	TArray<FTransform> Result;
	Result.Reserve(InteractionPoints.Num());

	const FTransform ComponentWorldTransform = GetComponentTransform();

	for (const FTransform& InteractionPoint : InteractionPoints)
	{
		Result.Add(InteractionPoint * ComponentWorldTransform);
	}

	return Result;
}

TOptional<FTransform> UInteractionComponent::GetClosestInteractionPoint(const FVector& Location) const
{
	float MinDistance = MAX_flt;
	TOptional<FTransform> ClosestPoint;
	
	const FTransform ComponentWorldTransform = GetComponentTransform();

	for (const FTransform& InteractionPoint : InteractionPoints)
	{
		FTransform WorldInteractionPoint = InteractionPoint * ComponentWorldTransform;
		
		const float Distance = FVector::Dist2D(Location, WorldInteractionPoint.GetLocation());
		if (Distance < MinDistance)
		{
			MinDistance = Distance;
			ClosestPoint = WorldInteractionPoint;
		}
	}

	return ClosestPoint;
}

void UInteractionComponent::UpdateWidgetData()
{
	if (InteractionWidgetComponent)
	{
		InteractionWidgetComponent->UpdateWidgetData();
	}
}

void UInteractionComponent::HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsActive)
	{
		return;
	}

	UInteractingComponent* InteractingComponent = OtherActor->GetComponentByClass<UInteractingComponent>();
	if (PlayerInteractionSubsystem && InteractingComponent)
	{
		PlayerInteractionSubsystem->RegisterInteraction(InteractingComponent, this);
	}

	if (InteractionWidgetComponent)
	{
		InteractionWidgetComponent->SetVisibility(true);
	}
}

void UInteractionComponent::HandleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!bIsActive)
	{
		return;
	}

	UInteractingComponent* InteractingComponent = OtherActor->GetComponentByClass<UInteractingComponent>();
	if (PlayerInteractionSubsystem && InteractingComponent)
	{
		PlayerInteractionSubsystem->RemoveInteraction(InteractingComponent);
	}

	if (InteractionWidgetComponent)
	{
		InteractionWidgetComponent->SetVisibility(false);
	}
}
