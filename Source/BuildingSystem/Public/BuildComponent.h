#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
