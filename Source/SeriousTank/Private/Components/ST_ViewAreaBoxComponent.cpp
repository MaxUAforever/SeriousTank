#include "Components/ST_ViewAreaBoxComponent.h"

#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h" 
#include "Kismet/GameplayStatics.h"

UST_ViewAreaBoxComponent::UST_ViewAreaBoxComponent()
{
	SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	SetUsingAbsoluteScale(true);
}

void UST_ViewAreaBoxComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner())
	{
		ParentCameraComponent = Cast<UCameraComponent>(GetOwner()->GetComponentByClass(UCameraComponent::StaticClass()));
		UpdateViewArea();
	}
}

void UST_ViewAreaBoxComponent::UpdateViewArea()
{
	if (!IsValid(ParentCameraComponent))
	{
		return;
	}

	FMinimalViewInfo DesiredView;
	ParentCameraComponent->GetCameraView(0.f, DesiredView);

	FMatrix ViewMatrix, ProjectionMatrix, ViewProjectionMatrix;
	UGameplayStatics::GetViewProjectionMatrix(DesiredView, ViewMatrix, ProjectionMatrix, ViewProjectionMatrix);

	FPlane FrustumPlanes[4];
	ViewProjectionMatrix.GetFrustumLeftPlane(FrustumPlanes[0]);
	ViewProjectionMatrix.GetFrustumTopPlane(FrustumPlanes[1]);
	ViewProjectionMatrix.GetFrustumRightPlane(FrustumPlanes[2]);
	ViewProjectionMatrix.GetFrustumBottomPlane(FrustumPlanes[3]);

	FVector Intersections[4];
	for (int i = 0; i < 4; ++i)
	{
		// Finding intersection between 2 planes of frustum.
		FVector Intersection, Direction;
		const bool bIsIntersected = FMath::IntersectPlanes2(Intersection, Direction, FrustumPlanes[i], FrustumPlanes[(i + 1) % 4]);

		if (!bIsIntersected)
		{
			UE_LOG(LogTemp, Warning, TEXT("UpdateCameraViewArea: Frustum planes are not intersected."));
			return;
		}

		// Calculate intersection point on 0z-axis.
		float ZeroZCoef = -Intersection.Z / Direction.Z;
		Intersections[i] = FVector{ Direction.X * ZeroZCoef + Intersection.X, Direction.Y * ZeroZCoef + Intersection.Y, 0.f };
	}

	// Because of perspective view it's not possible to represent view area in box shape, so we also capture "blind areas" on the sides.
	const FVector ExpandedSideViewPoint = { Intersections[3].X, Intersections[0].Y, 0.f };

	const float HalfWidth = FVector::Dist(Intersections[1], Intersections[0]) / 2;
	const float HalfLength = FVector::Dist(Intersections[0], ExpandedSideViewPoint) / 2;

	SetWorldLocation(FVector{ ExpandedSideViewPoint.X + HalfLength, ExpandedSideViewPoint.Y + HalfWidth, ViewAreaHight });
	SetBoxExtent(FVector{ HalfLength, HalfWidth, ViewAreaHight });

	FrontViewDistance = HalfLength * 2;
	OnFrontViewDistanceChanged.Broadcast(FrontViewDistance);

	if (bDrawDebug)
	{
		DrawDebugSphere(GetWorld(), GetComponentLocation(), 10.f, 12, FColor::Green, false, 30.f, 0U, 5.f);
		DrawDebugSphere(GetWorld(), ExpandedSideViewPoint, 10.f, 12, FColor::Red, false, 30.f, 0U, 5.f);
	
		DrawDebugLine(GetWorld(), Intersections[1], Intersections[0], FColor::Red, false, 30.f, 0U, 5.f);
		DrawDebugLine(GetWorld(), Intersections[0], ExpandedSideViewPoint, FColor::Green, false, 30.f, 0U, 5.f);
	}
}