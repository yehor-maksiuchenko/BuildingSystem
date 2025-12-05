#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BuildingTile_Master.h"
#include "TileDatabase.generated.h"

USTRUCT(BlueprintType)
struct FTile {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<ABuildingTile_Master> TileClass;
};

UCLASS()
class BUILDINGSYSTEM_API UTileDatabase : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FTile> Tiles;
};
