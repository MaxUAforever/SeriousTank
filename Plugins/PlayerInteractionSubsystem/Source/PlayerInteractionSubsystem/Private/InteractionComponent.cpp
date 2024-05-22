#include "InteractionComponent.h"

#include "Actions/BaseInteractionAction.h"
#include "InteractingComponent.h"
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
}

void UInteractionComponent::ActivateAction(UInteractingComponent* InteractingComponent)
{
	if (!InteractionAction)
	{
		UE_LOG(LogTemp, Warning, TEXT("InteractionComponent::ActivateAction: Action isn't set for component!"));
		return;
	}
		
	InteractionAction->Activate(InteractingComponent, this);
}

void UInteractionComponent::HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!InteractionAction)
	{
		UE_LOG(LogTemp, Warning, TEXT("InteractionComponent::HandleBeginOverlap: Action isn't set for component!"));
		return;
	}

	UInteractingComponent* InteractingComponent = OtherActor->GetComponentByClass<UInteractingComponent>();
	if (PlayerInteractionSubsystem && InteractingComponent)
	{
		PlayerInteractionSubsystem->RegisterInteraction(InteractingComponent, this);
	}
}

void UInteractionComponent::HandleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("InteractionComponent::HandleEndOverlap called"));

	UInteractingComponent* InteractingComponent = OtherActor->GetComponentByClass<UInteractingComponent>();
	if (PlayerInteractionSubsystem && InteractingComponent)
	{
		PlayerInteractionSubsystem->RemoveInteraction(InteractingComponent);
	}
}
