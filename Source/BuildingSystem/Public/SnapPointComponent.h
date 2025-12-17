#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "GameplayTagContainer.h"
#include "SnapPointComponent.generated.h"

// Forward Declaration
class UBoxComponent;
class USphereComponent;
class UCapsuleComponent;

UENUM(BlueprintType)
enum class ESnapPointShape : uint8
{
	BOX = 0		 UMETA(DisplayName = "Box"),
	SPHERE = 1   UMETA(DisplayName = "Sphere"),
	CAPSULE = 2  UMETA(DisplayName = "Capsule")
};

UENUM(BlueprintType)
enum class ESnapBehaviour : uint8
{
	SNAP_BASE_LOCATION = 0  UMETA(DisplayName = "Snap to Base Location", Tooltip = "Preview shall snap to the location of the Snap Point"),
	SNAP_EDGE = 1           UMETA(DisplayName = "Snap to the SnapPoint Edge", Tooltip = "Preview shall snap to the edge of the Snap Point")
};

USTRUCT(BlueprintType)
struct FSnapPointProperties {

	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Snap Point")
	ESnapPointShape ESPS_Shape;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Snap Point")
	FGameplayTagContainer SnapTags;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Snap Point")
	FVector SnapPointLocation;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Snap Point")
	float PRIORITY;

	FSnapPointProperties()
		: ESPS_Shape(ESnapPointShape::BOX)
		, SnapTags()
		, SnapPointLocation(FVector::ZeroVector)
		, PRIORITY(0.f)
	{
	}

	/** Convenience constructor for C++ use */
	FSnapPointProperties(
		ESnapPointShape InShape,
		const FGameplayTagContainer& InTags,
		const FVector& InLocation,
		float InPriority)
		: ESPS_Shape(InShape)
		, SnapTags(InTags)
		, SnapPointLocation(InLocation)
		, PRIORITY(InPriority)
	{
	}
};

USTRUCT(BlueprintType)
struct FSnappingRules {

	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FTransform SnapPointTransform;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	ESnapPointShape ECS_Shape;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	ESnapBehaviour ESB_SnapRules;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool bHardSnappingEnabled;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float HardSnappingRange;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool bSoftSnappingEnabled;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float SoftSnappingRange;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool bSuggestedRotationEnabled = false;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FRotator SnapRotation;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool bSuggestedScaleEnabled = false;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FVector SnapScale;

	FSnappingRules()
		: SnapPointTransform(FTransform::Identity)
		, ECS_Shape(ESnapPointShape::BOX)
		, ESB_SnapRules(ESnapBehaviour::SNAP_BASE_LOCATION)
		, bHardSnappingEnabled(false)
		, HardSnappingRange(0.f)
		, bSoftSnappingEnabled(false)
		, SoftSnappingRange(0.f)
		, bSuggestedRotationEnabled(false)
		, SnapRotation(FRotator::ZeroRotator)
		, bSuggestedScaleEnabled(false)
		, SnapScale(FVector::OneVector)
	{
	}

	FSnappingRules(
		const FTransform& Transform,
		ESnapPointShape Shape,
		ESnapBehaviour Behaviour,
		bool bHardSnapping,
		float HardRange,
		bool bSoftSnapping,
		float SoftRange,
		bool bRotationEnabled,
		const FRotator& Rotation,
		bool bScaleEnabled,
		const FVector& Scale)
		: SnapPointTransform(Transform)
		, ECS_Shape(Shape)
		, ESB_SnapRules(Behaviour)
		, bHardSnappingEnabled(bHardSnapping)
		, HardSnappingRange(HardRange)
		, bSoftSnappingEnabled(bSoftSnapping)
		, SoftSnappingRange(SoftRange)
		, bSuggestedRotationEnabled(bRotationEnabled)
		, SnapRotation(Rotation)
		, bSuggestedScaleEnabled(bScaleEnabled)
		, SnapScale(Scale)
	{
	}
};

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGSYSTEM_API USnapPointComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	USnapPointComponent();

	UFUNCTION(BlueprintCallable)
	void SnapPointInit(ESnapPointShape ECS_Shape, ESnapBehaviour ESB_SnapBehaviour, 
		FGameplayTagContainer SnapTags, float PRIORITY, bool bHardSnappingEnabled, 
		float HardSnappingRange, bool bSoftSnappingEnabled, float SoftSnappingRange, 
		bool bSuggestedRotationEnabled, FRotator SuggestedRotation, bool bSuggestedScaleEnabled, 
		FVector SuggestedScale);

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* Public Getters */
	UFUNCTION(BlueprintPure)
	FSnapPointProperties GetSnapPointProperties();

	UFUNCTION(BlueprintPure)
	FSnappingRules GetSnappingRules();

	UFUNCTION(BlueprintPure)
	UCapsuleComponent* GetCapsuleCollision() const { return CapsuleCollision; }

	UFUNCTION(BlueprintPure)
	UBoxComponent* GetBoxCollision() const { return BoxCollision; }

	UFUNCTION(BlueprintPure)
	USphereComponent* GetSphereCollision() const { return SphereCollision; }

protected:
										/* Variables */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ESnapPointShape ESPS_Shape; // Decides which shape will SnapPoint`s Collision take

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ESnapBehaviour ESB_SnapBehaviour;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FGameplayTagContainer SnapTags;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float PRIORITY = 1.0f; // Needed for Weighted Rule-Based Scoring

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bHardSnappingEnabled;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", 
		EditCondition = "bHardSnappingEnabled", EditConditionHides))
	float HardSnappingRange;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bSoftSnappingEnabled;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true",
		EditCondition = "bSoftSnappingEnabled", EditConditionHides))
	float SoftSnappingRange;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bSuggestedRotationEnabled;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true",
		EditCondition = "bSuggestedRotationEnabled", EditConditionHides))
	FRotator SuggestedRotation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bSuggestedScaleEnabled;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true",
		EditCondition = "bSuggestedScaleEnabled", EditConditionHides))
	FVector SuggestedScale;

	/* VISUALS */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", 
		EditCondition = "ESPS_Shape == ESnapPointShape::BOX", EditConditionHides))
	UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true",
		EditCondition = "ESPS_Shape == ESnapPointShape::SPHERE", EditConditionHides))
	USphereComponent* SphereCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true",
		EditCondition = "ESPS_Shape == ESnapPointShape::CAPSULE", EditConditionHides))
	UCapsuleComponent* CapsuleCollision;

};
