#include "SnapManagerSubsystem.h"
#include "SnapPointComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"

FTransform USnapManagerSubsystem::CalculateSnap(FTransform PreviewTransform, TMap<FGameplayTag, int32> TagPreferences)
{
	// Find all snap points in range. Sphere Trace is used, FOR NOW, since later on Snap Points will be cached in a spatial structure for fast lookup.
	TArray<UPrimitiveComponent*> OverlappedComponents;
	TArray<AActor*> ActorsToIgnore;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel2));

	UKismetSystemLibrary::SphereOverlapComponents(
		GetWorld(),
		PreviewTransform.GetLocation(),
		500.0f,
		ObjectTypes,
		nullptr,
		ActorsToIgnore,
		OverlappedComponents);

	TArray<USnapPointComponent*> AvailableSnapPoints;

	for (UPrimitiveComponent* Comp : OverlappedComponents)
	{
		if (!Comp) continue;

		if (USnapPointComponent* SnapPoint =
			Cast<USnapPointComponent>(Comp->GetAttachParent()))
		{
			AvailableSnapPoints.AddUnique(SnapPoint);
		}
	}

	// Getting Non-zero tag preferences
	FGameplayTagContainer RelevantTags;
	for (const TPair<FGameplayTag, int32>& Pair : TagPreferences)
	{
		if (Pair.Value != 0)
		{
			RelevantTags.AddTag(Pair.Key);
		}
	}

	SortSnapPoints(AvailableSnapPoints, RelevantTags);

	// Weighted Rule-Based Selection of best Snap Point
	TPair<USnapPointComponent*, int32> BestSnapPoint = TPair<USnapPointComponent*, int32>(nullptr, 0);
	for (USnapPointComponent* SnapComp : AvailableSnapPoints)
	{
		float SnapScore = CalculateSnapScore(PreviewTransform, SnapComp->GetSnapPointProperties(), TagPreferences);
		if (SnapScore > BestSnapPoint.Value)
		{
			BestSnapPoint.Key = SnapComp;
			BestSnapPoint.Value = SnapScore;
		}
	}	

	if ((!IsValid(BestSnapPoint.Key)) or (BestSnapPoint.Value <= 0))
	{
		return PreviewTransform;
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Emerald, FString::Printf(TEXT("Snap Score: %d;"), BestSnapPoint.Value));
	}
	return RegulateSnap(PreviewTransform, BestSnapPoint.Key);
}

void USnapManagerSubsystem::SortSnapPoints(TArray<USnapPointComponent*>& SnapsToSort, FGameplayTagContainer RelevantTags) const
{
	TArray<USnapPointComponent*> SortedSnapPoints;
	for (USnapPointComponent* Comp : SnapsToSort) {
		if (!Comp->GetSnapPointProperties().SnapTags.HasAnyExact(RelevantTags)) {
			SortedSnapPoints.Remove(Comp);
		}
	}
}

float USnapManagerSubsystem::CalculateSnapScore(const FTransform PreviewTransform, const FSnapPointProperties& SnapPointProperties, const TMap<FGameplayTag, int>& TagWeights) const
{
	float Score = 0;

	// 1. Accounting for Tags
	float TagScore = 0.f;
	int32 ConsideredTags = 0;

	for (const FGameplayTag& SnapTag : SnapPointProperties.SnapTags)
	{
		if (const int* Weight = TagWeights.Find(SnapTag))
		{
			if (*Weight > 0.f)
			{
				TagScore += *Weight;
				++ConsideredTags;
			}
		}
	}

	// Average tag score
	if (ConsideredTags > 0)
	{
		TagScore /= ConsideredTags;
		Score += TagScaleMultiplier * TagScore;
	}

	// 2. Accounting for distance to SnapPoint
	float Distance = FVector::Distance(PreviewTransform.GetLocation(), SnapPointProperties.SnapPointLocation);

	//float NormalizedDistance = FMath::Clamp(, 0.f, 1.f);

	Score += (1.f - (Distance / AvgSnapDistance)) * DistanceScaleMultiplier;

	// 3. Accounting for Priority
	Score += SnapPointProperties.PRIORITY * PriorityScaleMultiplier;

	return Score;
}

FTransform USnapManagerSubsystem::RegulateSnap(const FTransform& PreviewTransform, USnapPointComponent* SnapPoint)
{
	FTransform Result = PreviewTransform;
	const FSnappingRules& Rules = SnapPoint->GetSnappingRules();
	/* -------------------------------
	   1. LOCATION REGULATION
	   ------------------------------- */

	FVector SnappedLocation = PreviewTransform.GetLocation();

	switch (Rules.ESB_SnapRules)
	{
	case ESnapBehaviour::SNAP_BASE_LOCATION:
	{
		SnappedLocation = Rules.SnapPointTransform.GetLocation();
		break;
	}

	case ESnapBehaviour::SNAP_EDGE:
	{
		const FVector DirectionToSnapPoint = (PreviewTransform.GetLocation() - Rules.SnapPointTransform.GetLocation()).GetSafeNormal();

		switch (Rules.ECS_Shape)
		{
		case ESnapPointShape::BOX:
		{
			const FVector Extents = Rules.SnapPointTransform.GetScale3D() * 50.f;

			SnappedLocation = Rules.SnapPointTransform.GetLocation() + DirectionToSnapPoint * Extents.GetMax();
			break;
		}

		case ESnapPointShape::SPHERE:
		{
			const float Radius = Rules.SnapPointTransform.GetScale3D().X * 50.f;

			SnappedLocation = Rules.SnapPointTransform.GetLocation() + DirectionToSnapPoint * Radius;
			break;
		}

		case ESnapPointShape::CAPSULE:
		{
			const float Radius = SnapPoint->GetCapsuleCollision()->GetUnscaledCapsuleRadius();

			FVector SnapPointPreviewDirection = (PreviewTransform.GetLocation() - Rules.SnapPointTransform.GetLocation()).GetSafeNormal();

			SnappedLocation = Rules.SnapPointTransform.GetLocation() + SnapPointPreviewDirection * Radius;
			break;
		}
		}
		break;
	}
	}

	const float Distance = FVector::Dist(PreviewTransform.GetLocation(), SnappedLocation);
	if (!(Rules.bHardSnappingEnabled && Rules.HardSnappingRange >= Distance))
		SnappedLocation = PreviewTransform.GetLocation();

	Result.SetLocation(SnappedLocation);

	/* -------------------------------
	   2. ROTATION REGULATION
	   ------------------------------- */

	if (Rules.bSuggestedRotationEnabled)
	{
		Result.SetRotation(Rules.SnapRotation.Quaternion());
	}

	/* -------------------------------
	   3. SCALE REGULATION
	   ------------------------------- */

	if (Rules.bSuggestedScaleEnabled)
	{
		Result.SetScale3D(Rules.SnapScale);
	}

	return Result;
}