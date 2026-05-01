#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameModeType.h"
#include "SnakeGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SNAKE_ASSIGNMENT_API USnakeGameInstance : public UGameInstance
{
	GENERATED_BODY()
private:


public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GameMode")
	EGameModeType GameModeType;
};
