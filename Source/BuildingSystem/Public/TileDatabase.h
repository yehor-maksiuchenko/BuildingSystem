#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BuildingTile_Master.h"
#include "TileDatabase.generated.h"

UCLASS()
class BUILDINGSYSTEM_API UTileDatabase : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSubclassOf<ABuildingTile_Master>> Tiles;
};
