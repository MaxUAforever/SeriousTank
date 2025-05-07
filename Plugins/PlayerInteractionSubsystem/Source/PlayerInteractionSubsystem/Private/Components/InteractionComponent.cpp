#include "Components/InteractionComponent.h"

#include "Actions/BaseAutomaticAction.h"
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

	if (IsValid(PlayerInteractionSubsystem))
	{
		PlayerInteractionSubsystem->RegisterInteractionComponent(this);
	}

	if (bIsWidgetEnabled && InteractionWidgetClass)
	{
		InteractionWidgetComponent = NewObject<UInteractionWidgetComponent>(GetOwner(), UInteractionWidgetComponent::StaticClass(), TEXT("InteractionWidget"));
		if (IsValid(InteractionWidgetComponent))
		{
			InteractionWidgetComponent->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			InteractionWidgetComponent->RegisterComponent();

			InteractionWidgetComponent->SetVisibility(false);

			InteractionWidgetComponent->SetWidgetClass(InteractionWidgetClass);
			InteractionWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
			InteractionWidgetComponent->SetDrawAtDesiredSize(true);
			InteractionWidgetComponent->SetRelativeLocation(FVector{ 0.f, 0.f, 0.f });
			InteractionWidgetComponent->UpdateWidgetData();
		}
	}
}

void UInteractionComponent::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	if (IsValid(PlayerInteractionSubsystem))
	{
		PlayerInteractionSubsystem->UnregisterInteractionComponent(this);
	}

	Super::EndPlay(EndPlayReason);
}

void UInteractionComponent::SetIsInteractionComponentActive(bool bInIsActive)
{
	if (bIsActive == bInIsActive || !PlayerInteractionSubsystem)
	{
		return;
	}

	if (bInIsActive && !InteractionActionClass->IsChildOf(UBaseAutomaticAction::StaticClass()))
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
	OnIsComponentActiveChangedDelegate.ExecuteIfBound(this, bIsActive);

	if (InteractionWidgetComponent)
	{
		InteractionWidgetComponent->SetVisibility(bIsActive);
	}
}

bool UInteractionComponent::StopInteraction()
{
	if (!IsValid(PlayerInteractionSubsystem))
	{
		return false;
	}

	return PlayerInteractionSubsystem->StopInteractionAction(this);
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
		InteractingComponent->OnInteractionRegisterStateChangedDelegate.AddUObject(this, &ThisClass::OnInteractionStateChanged);
		PlayerInteractionSubsystem->RegisterInteraction(InteractingComponent, this);

		UpdateWidgetData();
	}
}

void UInteractionComponent::HandleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!bIsActive)
	{
		return;
	}

	UInteractingComponent* InteractingComponent = OtherActor->GetComponentByClass<UInteractingComponent>();
	if (IsValid(PlayerInteractionSubsystem) && InteractingComponent)
	{
		PlayerInteractionSubsystem->StopInteractionAction(this);
		PlayerInteractionSubsystem->RemoveInteraction(this);

		InteractingComponent->OnInteractionRegisterStateChangedDelegate.RemoveAll(this);
	}
}

void UInteractionComponent::OnInteractionStateChanged(bool bIsInteracting, const UInteractionComponent* InteractionComponent)
{
	if (InteractionWidgetComponent && InteractionComponent == this)
	{
		InteractionWidgetComponent->SetVisibility(bIsInteracting);
	}
}
