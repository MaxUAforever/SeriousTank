#include "InteractionComponent.h"

#include "Actions/BaseInteractionAction.h"
#include "Components/WidgetComponent.h"
#include "InteractingComponent.h"
#include "InteractionPointComponent.h"
#include "InteractionUserWidget.h"
#include "InteractionWidgetComponent.h"
#include "PlayerInteractionSubsystem.h"

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!InteractionActionClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("InteractionComponent::BeginPlay: InteractionActionClass isn't set"));
		return;
	}
	
	OnComponentBeginOverlap.AddDynamic(this, &ThisClass::HandleBeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &ThisClass::HandleEndOverlap);

	PlayerInteractionSubsystem = GetWorld() ? GetWorld()->GetSubsystem<UPlayerInteractionSubsystem>() : nullptr;
	InteractionAction = NewObject<UBaseInteractionAction>(this, InteractionActionClass.Get());

	if (ActionMontage)
	{
		InteractionAction->SetActionMontage(ActionMontage);
	}

	if (DeactivationMontage)
	{
		InteractionAction->SetDeactivationMontage(DeactivationMontage);
	}

	if (InteractionWidgetClass)
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
	for (USceneComponent* ChildComponent : ChildComponents)
	{
		if (UInteractionPointComponent* InteractionPoint = Cast<UInteractionPointComponent>(ChildComponent))
		{
			InteractionPoints.Add(InteractionPoint);
		}
	}
}

bool UInteractionComponent::ActivateAction(UInteractingComponent* InteractingComponent)
{
	if (!InteractionAction)
	{
		UE_LOG(LogTemp, Warning, TEXT("InteractionComponent::ActivateAction: Action isn't set for component!"));
		return false;
	}
		
	if (bIsActive)
	{
		return InteractionAction->Activate(InteractingComponent, this);
	}

	return false;
}

bool UInteractionComponent::DeactivateAction(UInteractingComponent* InteractingComponent)
{
	if (!InteractionAction)
	{
		UE_LOG(LogTemp, Warning, TEXT("InteractionComponent::ActivateAction: Action isn't set for component!"));
		return false;
	}

	return InteractionAction->Deactivate(InteractingComponent, this);
}

void UInteractionComponent::SetIsComponentActive(bool bInIsActive)
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

const UInteractionPointComponent* UInteractionComponent::GetClosestInteractionPoint(const FVector& Location) const
{
	float MinDistance = MAX_flt;
	UInteractionPointComponent* ClosestPoint = nullptr;
	
	for (UInteractionPointComponent* InteractionPoint : InteractionPoints)
	{
		const float Distance = FVector::Dist2D(Location, InteractionPoint->GetComponentLocation());
		if (Distance < MinDistance)
		{
			MinDistance = Distance;
			ClosestPoint = InteractionPoint;
		}
	}

	return ClosestPoint;
}

void UInteractionComponent::HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!InteractionAction)
	{
		UE_LOG(LogTemp, Warning, TEXT("InteractionComponent::HandleBeginOverlap: Action isn't set for component!"));
		return;
	}

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
