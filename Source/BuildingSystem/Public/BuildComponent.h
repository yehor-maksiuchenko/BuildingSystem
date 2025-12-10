#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "TileDatabase.h"
#include "BuildComponent.generated.h"

class UStaticMeshComponent;
class UCameraComponent;

DECLARE_DELEGATE_RetVal_TwoParams(FTransform, FPreviewSnappingRequestSignature, FVector /* TraceResultLocation */, FGameplayTagContainer /* SortingTags */);

UCLASS( Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGSYSTEM_API UBuildComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBuildComponent();

	FPreviewSnappingRequestSignature PreviewSnappingRequest;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void TryPlaceTheTile(const FInputActionValue& Value);

	UFUNCTION()
	void ToggleBuildMode(const FInputActionValue& Value);

	UFUNCTION()
	void SelectNextTile();

	UFUNCTION()
	void SelectPreviousTile();

	UFUNCTION()
	void SetPreviewMesh(UStaticMesh* NewMesh);

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// TAG-SHORTCUTSy. ONLY TO BE USED IN C++

	const FGameplayTag BuildModeTag = FGameplayTag::RequestGameplayTag(FName("PlayerState.BuildMode"));
	const FGameplayTag AdjustModeTag = FGameplayTag::RequestGameplayTag(FName("PlayerState.AdjustMode"));
	const FGameplayTag PreviewValidTag = FGameplayTag::RequestGameplayTag(FName("PreviewState.Valid"));
	const FGameplayTag PreviewInvalidTag = FGameplayTag::RequestGameplayTag(FName("PreviewState.Invalid"));
	const FGameplayTag PreviewPartialValidTag = FGameplayTag::RequestGameplayTag(FName("PreviewState.Partial"));

protected:
	// MISCHELANOUS

	UPROPERTY(BlueprintReadOnly)
	UCameraComponent* OwnerCamera;

	UPROPERTY(BlueprintReadWrite)
	FGameplayTagContainer StateTags;

	// INPUT

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputAction* ToggleBuildModeAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputAction* PlaceTheTileAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputAction* NextTileAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputAction* PreviousTileAction;

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
