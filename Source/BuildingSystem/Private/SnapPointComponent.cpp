#include "SnapPointComponent.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/PrimitiveComponent.h"

USnapPointComponent::USnapPointComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));

	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollision"));
}

// Init function with custom input since Components don't support deferred spawn. Bit ugly, which I hate, but hopefully a fair trade-off
void USnapPointComponent::SnapPointInit(ESnapPointShape ECS_Shape_NEW, ESnapBehaviour SnapBehaviour_NEW, FGameplayTagContainer SnapTags_NEW, float PRIORITY_NEW, bool bHardSnappingEnabled_NEW, float HardSnappingRange_NEW, bool bSoftSnappingEnabled_NEW, float SoftSnappingRange_NEW, bool bSuggestedRotationEnabled_NEW, FRotator SuggestedRotation_NEW, bool bSuggestedScaleEnabled_NEW, FVector SuggestedScale_NEW)
{
	ESPS_Shape = ECS_Shape_NEW;
	ESB_SnapBehaviour = SnapBehaviour_NEW;
	SnapTags = SnapTags_NEW;
	PRIORITY = PRIORITY_NEW;
	bHardSnappingEnabled = bHardSnappingEnabled_NEW;
	HardSnappingRange = HardSnappingRange_NEW;
	bSoftSnappingEnabled = bSoftSnappingEnabled_NEW;
	SoftSnappingRange = SoftSnappingRange_NEW;
	bSuggestedRotationEnabled = bSuggestedRotationEnabled_NEW;
	SuggestedRotation = SuggestedRotation_NEW;
	bSuggestedScaleEnabled = bSuggestedScaleEnabled_NEW;
	SuggestedScale = SuggestedScale_NEW;
}

void USnapPointComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void USnapPointComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

FSnapPointProperties USnapPointComponent::GetSnapPointProperties()
{
	return FSnapPointProperties(ESPS_Shape, SnapTags, USceneComponent::GetComponentLocation(), PRIORITY);
}

FSnappingRules USnapPointComponent::GetSnappingRules()
{
	return FSnappingRules(USceneComponent::GetComponentTransform(), ESPS_Shape, ESB_SnapBehaviour, bSuggestedRotationEnabled, SuggestedRotation, bSuggestedScaleEnabled, SuggestedScale);
}

