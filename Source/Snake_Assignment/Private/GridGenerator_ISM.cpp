// GridGeneratorISM.cpp

#include "GridGenerator_ISM.h"
#include "NavigationSystem.h"
#include "NavAreas/NavArea_Null.h"

AGridGeneratorISM::AGridGeneratorISM()
{
    PrimaryActorTick.bCanEverTick = false;

    // Create ISM components
    FloorISM = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("FloorISM"));
    WallISM = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("WallISM"));

    RootComponent = FloorISM;
    WallISM->SetupAttachment(RootComponent);
}

void AGridGeneratorISM::BeginPlay()
{
    Super::BeginPlay();

    GenerateGrid();
    UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
    if (NavSys)
    {
        NavSys->Build();
    }

    for (int i = FoodAmount; i < MaxFoodAmount; i++)
    {
        SpawnFoodOnNavMesh();
    }
}

void AGridGeneratorISM::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

	GenerateGrid();
}

void AGridGeneratorISM::AddFoodCount()
{
	FoodAmount++;
}

void AGridGeneratorISM::GenerateGrid()
{
    if (!FloorMesh || !WallMesh) return;

    // Assign meshes
    FloorISM->SetStaticMesh(FloorMesh);
    WallISM->SetStaticMesh(WallMesh);
	FloorISM->SetCanEverAffectNavigation(true);
	WallISM->SetCanEverAffectNavigation(false);
	WallISM->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	WallISM->SetCollisionResponseToAllChannels(ECR_Block);

    // Clear old instances (important when editing in editor)
	FloorISM->ClearInstances();
	WallISM->ClearInstances();
	FloorPositions.Empty();

    for (int32 x = 0; x < GridWidth; x++)
    {
        for (int32 y = 0; y < GridHeight; y++)
        {
            FVector Location = FVector(x * TileSize, y * TileSize, 0.f);
            FTransform Transform(Location);

            bool bIsEdge =
                (x == 0 || x == GridWidth - 1 ||
                    y == 0 || y == GridHeight - 1);

            if (bIsEdge)
            {
                WallISM->AddInstance(Transform);
            }
            else
            {
                FVector Center(GridWidth / 2, GridHeight / 2, 0);

                float SafeRadiusWidth = 8;
				float SafeRadiusHeight = 5;

                bool bNearCenter =
                    FMath::Abs(x - GridWidth / 2) < SafeRadiusWidth &&
                    FMath::Abs(y - GridHeight / 2) < SafeRadiusHeight;

                if(!bNearCenter && FMath::FRand() < ObstacleChance)
                {
                    WallISM->AddInstance(Transform);
				}
                else 
                {
                    FloorISM->AddInstance(Transform);
                    FloorPositions.Add(Location * 2);
                }
            }
        }
    }
}

void AGridGeneratorISM::SpawnFoodOnNavMesh()
{
    if (!SpawnActorClass) return;
    UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
    if (!NavSys) return;
    FNavLocation RandomPoint;
    bool bFound = NavSys->GetRandomPoint(RandomPoint);
    if (bFound)
    {
        GetWorld()->SpawnActor<AActor>(
            SpawnActorClass,
            RandomPoint.Location,
            FRotator::ZeroRotator
        );
    }
}

void AGridGeneratorISM::RePositionFoodOnNavMesh(AActor* Actor) 
{
	if (!SpawnActorClass) return;
    UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
    if (!NavSys) return;
    FNavLocation RandomPoint;
    bool bFound = NavSys->GetRandomPoint(RandomPoint);
	if (bFound)
        {
        // Assuming you have a reference to the food actor you want to reposition
        AActor* FoodActor = Actor; // Replace with your actual food actor reference
        if (FoodActor)
        {
            FoodActor->SetActorLocation(RandomPoint.Location);
        }
	}
}

void AGridGeneratorISM::RePositionActor(AActor* Actor)
{
    // This function can be called to reposition an actor to a new random location on the grid
    if (!SpawnActorClass || FloorPositions.Num() == 0) return;
	int32 RandomIndex = FMath::RandRange(0, FloorPositions.Num() - 1);
    //FVector SpawnHit = FMath::RandPointInBox(FBox(FVector(1, 1, 0), FVector(GridWidth * TileSize * 2 -200, GridHeight * TileSize * 2 - 200, 0)));
	FVector NewLocation = FloorPositions[RandomIndex];
    //NewLocation.Z += 50.f; // Optional: lift slightly above ground
    // Assuming you have a reference to the actor you want to reposition
	AActor* ActorToMove = Actor; // Replace with your actual actor reference
    if (ActorToMove)
    {
        ActorToMove->SetActorLocation(NewLocation);
    }
}

