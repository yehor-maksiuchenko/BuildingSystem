#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameplayTagContainer.h"
#include "SnapManagerSubsystem.generated.h"



UCLASS()
class BUILDINGSYSTEM_API USnapManagerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	FTransform CalculateSnap(FTransform PreviewTransform, TMap<FGameplayTag, int32> TagPreferences);

private:
	UFUNCTION(BlueprintPure)
	void SortSnapPoints(TArray<USnapPointComponent*>& SnapsToSort, FGameplayTagContainer RelevantTags) const;

	UFUNCTION(BlueprintCallable)
	float CalculateSnapScore(const FTransform PreviewTransform, const FSnapPointProperties& SnapPoint, const TMap<FGameplayTag, int>& TagWeights) const;

	UFUNCTION(BlueprintPure)
	FTransform RegulateSnap(const FTransform& PreviewTransform, USnapPointComponent* SnapPoint);

	UPROPERTY()
	float TagScaleMultiplier = 10.0f;

	UPROPERTY()
	float DistanceScaleMultiplier = 0.2f;

	UPROPERTY()
	float PriorityScaleMultiplier = 2.0f;

	UPROPERTY()
	float AvgSnapDistance = 200.f;
};
