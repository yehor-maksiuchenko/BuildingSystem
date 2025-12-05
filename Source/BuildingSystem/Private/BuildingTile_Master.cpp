#include "BuildingTile_Master.h"

#include "Components/StaticMeshComponent.h"
#include "SnapPointComponent.h"

ABuildingTile_Master::ABuildingTile_Master()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
}

void ABuildingTile_Master::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABuildingTile_Master::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

ETileName ABuildingTile_Master::GetTileName()
{
	return ETN_TileName;
}

