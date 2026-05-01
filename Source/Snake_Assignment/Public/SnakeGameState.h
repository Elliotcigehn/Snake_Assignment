// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SnakeGameState.generated.h"

/**
 * 
 */
UCLASS()
class SNAKE_ASSIGNMENT_API ASnakeGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	bool bIsGameOver = false;

	UFUNCTION()
	void SetGameOver(bool bNewGameOver);
	
};
