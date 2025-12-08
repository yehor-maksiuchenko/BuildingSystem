#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BuildingTile_Master.h"
#include "GameplayTagContainer.h"
#include "BuildComponent.generated.h"

DECLARE_DELEGATE_RetVal_ThreeParams(FTransform, FPreviewSnappingRequestSignature, FVector /* StartLocation */, FVector /* ForwardVector */, float /* ForwardLength */);

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

private:
	struct FMyTags
	{
		static const FGameplayTag bInBuildMode;
		static const FGameplayTag bInAdjustMode;
		static const FGameplayTag PreviewValid;
		static const FGameplayTag PreviewInvalid;
		static const FGameplayTag PreviewPartial;
	};

	const FGameplayTag FMyTags::bInBuildMode = FGameplayTag::RequestGameplayTag(FName("PlayerState.BuildMode"));
	const FGameplayTag FMyTags::bInAdjustMode = FGameplayTag::RequestGameplayTag(FName("PlayerState.AdjustMode"));
	const FGameplayTag FMyTags::PreviewValid = FGameplayTag::RequestGameplayTag(FName("PreviewState.Valid"));
	const FGameplayTag FMyTags::PreviewInvalid = FGameplayTag::RequestGameplayTag(FName("PreviewState.Invalid"));
	const FGameplayTag FMyTags::PreviewPartial = FGameplayTag::RequestGameplayTag(FName("PreviewState.Partial"));

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer StateTags;

	// INPUT
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputAction* ToggleBuildModeAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputAction* PlaceTheTileAction;
};
