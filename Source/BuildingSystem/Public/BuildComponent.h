#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "TileDatabase.h"
#include "BuildComponent.generated.h"

class UStaticMeshComponent;

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

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer StateTags;

	// INPUT
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputAction* ToggleBuildModeAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputAction* PlaceTheTileAction;

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
};
