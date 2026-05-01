#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SnakeGameInstance.generated.h"

UENUM(BlueprintType)
enum class EGameModeType : uint8
{
    SinglePlayer,
    Coop
};
