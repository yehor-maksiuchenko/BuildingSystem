#include "BuildingTile_Master.h"

#include "Components/StaticMeshComponent.h"
#include "SnapPointComponent.h"

ABuildingTile_Master::ABuildingTile_Master()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = MeshComponent;
}

void ABuildingTile_Master::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABuildingTile_Master::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UStaticMeshComponent* ABuildingTile_Master::GetStaticMeshComponent() const
{
	return MeshComponent;
}

ETileName ABuildingTile_Master::GetTileName() const
{
	return ETN_TileName;
}

TArray<USnapPointComponent*> ABuildingTile_Master::GetSnapPoints()
{
	TArray<USnapPointComponent*> SnapPoints;
	GetComponents<USnapPointComponent>(SnapPoints);
	return SnapPoints;
}

