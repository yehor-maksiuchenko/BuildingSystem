#include "BuildComponent.h"
#include "EnhancedInputComponent.h"
#include "BuildingTile_Master.h"
#include "Engine/World.h" 
#include "SnapManagerSubsystem.h"
#include "BuildingSystemCharacter.h"
#include "Camera/CameraComponent.h"
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

    SetPreviewMesh(TilesList->GetTileMesh(CurrentTileSelection));
    PreviewMID = UMaterialInstanceDynamic::Create(PreviewMaterial, this);
    PreviewMesh->SetMaterial(0, PreviewMID);
    SetPreviewMaterialPrecise(1.0f);
    PreviewMesh->SetVisibility(false);
    HintPreviewMesh->SetVisibility(false);

    if (ABuildingSystemCharacter* Owner = Cast<ABuildingSystemCharacter>(GetOwner())) {
        OwnerCamera = Owner->GetFirstPersonCameraComponent();
    }

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
            InputComp->BindAction(ToggleBuildModeAction, ETriggerEvent::Started, this, &UBuildComponent::ToggleBuildMode);
            InputComp->BindAction(PlaceTheTileAction, ETriggerEvent::Started, this, &UBuildComponent::TryPlaceTheTile);
            InputComp->BindAction(NextTileAction, ETriggerEvent::Started, this, &UBuildComponent::SelectNextTile);
            InputComp->BindAction(PreviousTileAction, ETriggerEvent::Started, this, &UBuildComponent::SelectPreviousTile);
            InputComp->BindAction(RotatePreviewAction, ETriggerEvent::Started, this, &UBuildComponent::RotatePreview);
        }
    }
}

void UBuildComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (StateTags.HasTagExact(BuildModeTag)) {
        FHitResult HitResult = LineTrace();
        if (HitResult.bBlockingHit) {
            PreviewMesh->SetWorldLocation(HitResult.Location);
        }
        else {
            PreviewMesh->SetWorldLocation(HitResult.TraceEnd);
        }
    }

    if (PreviewSnappingRequest.IsBound())
    {
        PreviewMesh->SetWorldTransform(PreviewSnappingRequest.Execute(PreviewMesh->GetComponentTransform(), TilesList->GetTileTagPreferences(CurrentTileSelection)));
    }
}

float UBuildComponent::GetPreviewRotationSensitivity() const
{
    return PreviewRotationMultiplier;
}

void UBuildComponent::SetPreviewRotationSensitivity(float Value)
{
    PreviewRotationMultiplier = Value;
}

void UBuildComponent::SetPreviewMesh(UStaticMesh* NewMesh)
{
    if (NewMesh) {
        PreviewMesh->SetStaticMesh(NewMesh);
        HintPreviewMesh->SetStaticMesh(NewMesh);

        PreviewMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        HintPreviewMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

        PreviewMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
        HintPreviewMesh->SetCollisionResponseToAllChannels(ECR_Ignore);

        PreviewMesh->SetGenerateOverlapEvents(false);
        HintPreviewMesh->SetGenerateOverlapEvents(false);

        PreviewMesh->SetCanEverAffectNavigation(false);
        HintPreviewMesh->SetCanEverAffectNavigation(false);
    }
}

void UBuildComponent::TryPlaceTheTile(const FInputActionValue& Value)
{
    if (StateTags.HasTag(BuildModeTag)) {
        TSubclassOf<ABuildingTile_Master> TileClass = TilesList->GetTileClassRef(CurrentTileSelection);
        FActorSpawnParameters SpawnParams;
        GetWorld()->SpawnActor<ABuildingTile_Master>(
            TileClass,
            PreviewMesh->GetComponentTransform(),
            SpawnParams
        );
    }
}

void UBuildComponent::ToggleBuildMode(const FInputActionValue& Value)
{
    
    if (StateTags.HasTagExact(BuildModeTag))
    {
        PreviewMesh->SetVisibility(false);
        HintPreviewMesh->SetVisibility(false);
        StateTags.RemoveTag(BuildModeTag);
    }
    else {
        PreviewMesh->SetVisibility(true);
        HintPreviewMesh->SetVisibility(false);
        StateTags.AddTag(BuildModeTag);
    }
}

void UBuildComponent::SelectNextTile()
{
    if (!StateTags.HasTagExact(AdjustModeTag)) {
        if (CurrentTileSelection == (TilesList->Length() - 1)) {
            CurrentTileSelection = 0;
        }
        else {
            CurrentTileSelection++;
        }
        SetPreviewMesh(TilesList->GetTileMesh(CurrentTileSelection));
    }
}

void UBuildComponent::SelectPreviousTile()
{
    if (!StateTags.HasTagExact(AdjustModeTag)) {
        if (CurrentTileSelection == 0) {
            CurrentTileSelection = TilesList->Length() - 1;
        }
        else {
            CurrentTileSelection--;
        }
        SetPreviewMesh(TilesList->GetTileMesh(CurrentTileSelection));
    }
}

void UBuildComponent::RotatePreview(const FInputActionValue& Value)
{
    if (StateTags.HasTagExact(BuildModeTag) && !StateTags.HasTagExact(AdjustModeTag)) {
        FRotator CurrentRotation = PreviewMesh->GetComponentRotation();
        float NewYaw = CurrentRotation.Yaw + (PreviewRotationMultiplier * Value.GetMagnitude());
        PreviewMesh->SetWorldRotation(FRotator(CurrentRotation.Pitch, NewYaw, CurrentRotation.Roll));
    }
}

FHitResult UBuildComponent::LineTrace()
{
    FVector StartLocation = OwnerCamera->GetComponentLocation();
    FVector ForwardVector = OwnerCamera->GetForwardVector();
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

    return HitResult;
}

void UBuildComponent::SetPreviewMaterialPrecise(float NewValue)
{
    PreviewMID->SetScalarParameterValue(FName("Value"), FMath::Clamp(NewValue, 0.0f, 1.0f));
}

void UBuildComponent::SetPreviewMaterialDynamic(float MinDistance, float MaxDistance, float GivenDistance)
{

}

