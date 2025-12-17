#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BuildingTile_Master.h"
#include "TileDatabase.generated.h"

UCLASS()
class BUILDINGSYSTEM_API UTileDatabase : public UDataAsset
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSubclassOf<ABuildingTile_Master>> Tiles;

public:
	UFUNCTION(BlueprintPure)
	int Length() const;

	UFUNCTION(BlueprintPure)
	ETileName GetTileName(int Index) const;

	UFUNCTION(BlueprintPure)
	UStaticMesh* GetTileMesh(int Index) const;

	UFUNCTION(BlueprintPure)
	TSubclassOf<ABuildingTile_Master> GetTileClassRef(int Index) const;

	UFUNCTION(BlueprintPure)
	TArray<USnapPointComponent*> GetTileSnapPoints(int Index);

	UFUNCTION(BlueprintPure)
	TMap<FGameplayTag, int32> GetTileTagPreferences(int Index) const;
};
