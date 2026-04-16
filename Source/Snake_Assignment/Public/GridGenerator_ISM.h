// GridGeneratorISM.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "GridGenerator_ISM.generated.h"

UCLASS()
class AGridGeneratorISM : public AActor
{
    GENERATED_BODY()

public:
    AGridGeneratorISM();

protected:
    virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;

public:
    UPROPERTY(EditAnywhere, Category = "Grid")
    int32 GridWidth = 20;

    UPROPERTY(EditAnywhere, Category = "Grid")
    int32 GridHeight = 20;

    UPROPERTY(EditAnywhere, Category = "Grid")
    float TileSize = 100.f;

    UPROPERTY(EditAnywhere, Category = "Grid")
    UStaticMesh* FloorMesh;

    UPROPERTY(EditAnywhere, Category = "Grid")
    UStaticMesh* WallMesh;

	UPROPERTY(EditAnywhere, Category = "Grid")
	TSubclassOf<AActor> SpawnActorClass;

    UPROPERTY(EditAnywhere)
	TArray<FVector> FloorPositions;
    
private:
    UPROPERTY(EditAnywhere)
    UInstancedStaticMeshComponent* FloorISM;

    UPROPERTY(EditAnywhere)
    UInstancedStaticMeshComponent* WallISM;

    void GenerateGrid();
    UFUNCTION(BlueprintCallable)
	void SpawnRandomActor();
    UFUNCTION(BlueprintCallable)
	void RePositionActor(AActor* Actor);
};
