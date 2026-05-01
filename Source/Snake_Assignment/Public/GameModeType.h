// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Class.h"
#include "GameModeType.generated.h"

/**
 * 
 */
UCLASS()
class SNAKE_ASSIGNMENT_API UGameModeType : public UEnum
{
	GENERATED_BODY()
};

UENUM(BlueprintType)
enum class EGameModeType : uint8
{
	SinglePlayer,
	Coop
};


