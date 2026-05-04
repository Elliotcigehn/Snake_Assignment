#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SnakeGameMode.generated.h"

UCLASS()
class SNAKE_ASSIGNMENT_API ASnakeGameMode : public AGameModeBase
{
    GENERATED_BODY()

protected:
    ASnakeGameMode();
    virtual void BeginPlay() override;

public:
    UPROPERTY(EditAnywhere, Category = "Game")
    TSubclassOf<APawn> SnakePawnClass;

    UFUNCTION(BlueprintCallable)
	void TriggerGameOver();

	AActor* FindPlayerStartByTag(FName Tag);

private:
    void StartSinglePlayer();
    void StartCoop();
};
