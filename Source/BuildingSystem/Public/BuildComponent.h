#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BuildingTile_Master.h"
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
	void TryPlaceTheTile();

	UFUNCTION()
	void ToggleBuildMode();

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTileDatabase* TilesList;

	UFUNCTION(BlueprintCallable)
	FHitResult LineTrace();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector TraceResultLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* PreviewMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* HintPreviewMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintRead)
	UTileDatabase* TilesList;
};
