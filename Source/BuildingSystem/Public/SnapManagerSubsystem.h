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

protected:
	UFUNCTION(BlueprintPure)
	void SortSnapPoints(TArray<USnapPointComponent*>& SnapsToSort, FGameplayTagContainer RelevantTags) const;

	UFUNCTION(BlueprintCallable)
	float CalculateSnapScore(const FSnapPointProperties& SnapPoint, const TMap<FGameplayTag, int>& TagWeights) const;

	UFUNCTION(BlueprintPure)
	FTransform RegulateSnap(const FTransform& PreviewTransform, USnapPointComponent* SnapPoint);
};
