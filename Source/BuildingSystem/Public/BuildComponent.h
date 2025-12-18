#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "TileDatabase.h"
#include "BuildComponent.generated.h"

class UStaticMeshComponent;
class UCameraComponent;

using FTagPreferenceMap = TMap<FGameplayTag, int32>;

DECLARE_DELEGATE_RetVal_TwoParams(FTransform, FPreviewSnappingRequestSignature, FTransform /* PreviewTransform */, FTagPreferenceMap /* TagPreferences */)

UCLASS( Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGSYSTEM_API UBuildComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBuildComponent();

	FPreviewSnappingRequestSignature PreviewSnappingRequest;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable)
	void Server_PlaceTile(FTransform Transform, int32 TileIndex);

	UFUNCTION()
	void SetPreviewMesh(UStaticMesh* NewMesh);

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void TryPlaceTheTile(const FInputActionValue& Value);

	UFUNCTION()
	void ToggleBuildMode(const FInputActionValue& Value);

	UFUNCTION()
	void SelectNextTile();

	UFUNCTION()
	void SelectPreviousTile();

	UFUNCTION()
	void RotatePreview(const FInputActionValue& Value);

private:
	// TAG-SHORTCUTS. ONLY TO BE USED IN C++

	const FGameplayTag BuildModeTag = FGameplayTag::RequestGameplayTag(FName("PlayerState.BuildMode"));
	const FGameplayTag AdjustModeTag = FGameplayTag::RequestGameplayTag(FName("PlayerState.AdjustMode"));
	const FGameplayTag PreviewValidTag = FGameplayTag::RequestGameplayTag(FName("PreviewState.Valid"));
	const FGameplayTag PreviewInvalidTag = FGameplayTag::RequestGameplayTag(FName("PreviewState.Invalid"));
	const FGameplayTag PreviewPartialValidTag = FGameplayTag::RequestGameplayTag(FName("PreviewState.Partial"));

protected:
	// Player Settings

	UPROPERTY(EditDefaultsOnly)
	float PreviewRotationMultiplier = 5.0f;

	UFUNCTION(BlueprintPure)
	float GetPreviewRotationSensitivity() const;

	UFUNCTION(BlueprintCallable)
	void SetPreviewRotationSensitivity(float Value);

	// MISCHELANOUS

	UPROPERTY(BlueprintReadOnly)
	UCameraComponent* OwnerCamera;

	UPROPERTY(BlueprintReadWrite)
	FGameplayTagContainer StateTags;

	// Working with tiles

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTileDatabase* TilesList;

	UFUNCTION(BlueprintCallable)
	FHitResult LineTrace();

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	FVector TraceResultLocation;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	int CurrentTileSelection = 0;

	// Preview Mesh

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* PreviewMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* HintPreviewMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UMaterialInstance* PreviewMaterial;

	UPROPERTY(BlueprintReadOnly)
	UMaterialInstanceDynamic* PreviewMID;

	UFUNCTION(BlueprintCallable)
	void SetPreviewMaterialPrecise(float Value);

	UFUNCTION(BlueprintCallable)
	void SetPreviewMaterialDynamic(float MinDistance, float MaxDistance, float GivenDistance);
};
