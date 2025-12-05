#include "BuildComponent.h"
#include "SnapManagerSubsystem.h"

UBuildComponent::UBuildComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UBuildComponent::BeginPlay()
{
	Super::BeginPlay();

    if (UWorld* World = GetWorld())
    {
        if (USnapManagerSubsystem* SnapSubsystem = World->GetSubsystem<USnapManagerSubsystem>())
        {
            PreviewSnappingRequest.BindUObject(
                SnapSubsystem,
                &USnapManagerSubsystem::CalculateSnap
            );
        }
    }
}


void UBuildComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

