#include "BuildComponent.h"
#include "EnhancedInputComponent.h"
#include "BuildingTile_Master.h"
#include "Engine/World.h" 
#include "SnapManagerSubsystem.h"
#include "Components/StaticMeshComponent.h"

UBuildComponent::UBuildComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

    PreviewMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PreviewMesh"));

    HintPreviewMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HintPreviewMesh"));
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
    if (StateTags.HasTag(FGameplayTag::RequestGameplayTag(FName("PlayerState.BuildMode")))) {
        GetWorld()->SpawnActor<TSubclassOf<ABuildingTile_Master>>();
    }
}

void UBuildComponent::ToggleBuildMode()
{

}


void UBuildComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    FHitResult HitResult = LineTrace();
    if (HitResult.bBlockingHit) {

    }
}

FHitResult UBuildComponent::LineTrace()
{
    FVector StartLocation = GetOwner()->GetActorLocation();
    FVector ForwardVector = GetOwner()->GetActorForwardVector();
    float TraceLength = 500.f;

    FVector EndLocation = StartLocation + ForwardVector * TraceLength;

    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(GetOwner());

    GetWorld()->LineTraceSingleByChannel(
        HitResult,
        StartLocation,
        EndLocation,
        ECC_Visibility,
        QueryParams
    );

    return FHitResult();
}

