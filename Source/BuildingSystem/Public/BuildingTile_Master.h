#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildingTile_Master.generated.h"

class UStaticMeshComponent;
class USnapPointComponent;

UENUM(BlueprintType)
enum class ETileName : uint8 {
	Pile = 0  UMETA(DisplayName = "Pile", Tooltip = "A basic wooden pile"),
	Floor = 1 UMETA(DisplayName = "Floor", Tooltip = "A basic wooden floor. Can serve as a roof as well")
};

UCLASS(Abstract)
class BUILDINGSYSTEM_API ABuildingTile_Master : public AActor
{
	GENERATED_BODY()
	
public:	
	ABuildingTile_Master();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ETileName ETN_TileName;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure)
	virtual TArray<USnapPointComponent*> GetSnapPoints() PURE_VIRTUAL(ABuildingTile_Master::GetSnapPoints, return TArray<USnapPointComponent*>(););
};
