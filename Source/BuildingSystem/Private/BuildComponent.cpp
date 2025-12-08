#include "BuildComponent.h"
#include "EnhancedInputComponent.h"
#include "BuildingTile_Master.h"
#include "SnapManagerSubsystem.h"

UBuildComponent::UBuildComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UBuildComponent::BeginPlay()
{
	Super::BeginPlay();

    // Bind delegate to SnapManagerSubsystem
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

    // Bind 
    if (AActor* Owner = GetOwner())
    {
        if (UEnhancedInputComponent* InputComp = Owner->FindComponentByClass<UEnhancedInputComponent>())
        {
            InputComp->BindAction(ToggleBuildModeAction, ETriggerEvent::Started, this, &ToggleBuildMode);
            InputComp->BindAction(PlaceTheTileAction, ETriggerEvent::Started, this, &TryPlaceTheTile);
        }
    }
}

void UBuildComponent::TryPlaceTheTile()
{
    if (StateTags.HasTag(FMyTags::bInBuildMode)) {
        GetWorld()->SpawnActor<TSubclassOf<ABuildingTile_Master>>();
    }
}

void UBuildComponent::ToggleBuildMode()
{

}


void UBuildComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

