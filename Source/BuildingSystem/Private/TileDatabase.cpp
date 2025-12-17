#include "TileDatabase.h"

int UTileDatabase::Length() const 
{
    return Tiles.Num();
}

ETileName UTileDatabase::GetTileName(int Index) const
{
    const ABuildingTile_Master* CDO = Tiles[Index]->GetDefaultObject<ABuildingTile_Master>();
    return CDO->GetTileName();
}

UStaticMesh* UTileDatabase::GetTileMesh(int Index) const
{
    const ABuildingTile_Master* CDO = Tiles[Index]->GetDefaultObject<ABuildingTile_Master>();
    return CDO->GetStaticMeshComponent()->GetStaticMesh();
}

TSubclassOf<ABuildingTile_Master> UTileDatabase::GetTileClassRef(int Index) const
{
    return Tiles[Index];
}

TArray<USnapPointComponent*> UTileDatabase::GetTileSnapPoints(int Index)
{
    return TArray<USnapPointComponent*>();
}

TMap<FGameplayTag, int32> UTileDatabase::GetTileTagPreferences(int Index) const
{
    const ABuildingTile_Master* CDO = Tiles[Index]->GetDefaultObject<ABuildingTile_Master>();
    return CDO->GetTagPreferences();
}
