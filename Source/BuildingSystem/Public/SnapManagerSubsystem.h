#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "SnapPointComponent.h"
#include "SnapManagerSubsystem.generated.h"



UCLASS()
class BUILDINGSYSTEM_API USnapManagerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	FTransform CalculateSnap(FVector StartLocation, FVector ForwardVector, float ForwardLength);

};
