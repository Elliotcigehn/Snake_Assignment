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
	float ObstacleChance = 0.2f;

    UPROPERTY(EditAnywhere, Category = "Grid")
    UStaticMesh* FloorMesh;

    UPROPERTY(EditAnywhere, Category = "Grid")
    UStaticMesh* WallMesh;

	UPROPERTY(EditAnywhere, Category = "Grid")
	TSubclassOf<AActor> SpawnActorClass;

    UPROPERTY(EditAnywhere)
	TArray<FVector> FloorPositions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int FoodAmount = 0;

	UPROPERTY(EditAnywhere)
    int MaxFoodAmount = 6;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Score = 0;
    

    
private:
    UPROPERTY(EditAnywhere)
    UInstancedStaticMeshComponent* FloorISM;

    UPROPERTY(EditAnywhere)
    UInstancedStaticMeshComponent* WallISM;

    void GenerateGrid();
    UFUNCTION(BlueprintCallable)
	void SpawnFoodOnNavMesh();
    UFUNCTION(BlueprintCallable)
	void RePositionFoodOnNavMesh(AActor* Actor);
    UFUNCTION(BlueprintCallable)
	void AddFoodCount();
	UFUNCTION(BlueprintCallable)
	void RePositionActor(AActor* Actor);
	UFUNCTION(BlueprintCallable)
    void SetScore(int NewScore);
};
