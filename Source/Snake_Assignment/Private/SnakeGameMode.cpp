#include "SnakeGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "SnakeGameInstance.h"
#include "GameFramework/PlayerController.h"
#include "SnakeGameState.h" 

void ASnakeGameMode::BeginPlay()
{
    Super::BeginPlay();

    USnakeGameInstance* GI = Cast<USnakeGameInstance>(GetGameInstance());
    if (!GI) return;

    switch (GI->GameModeType)
    {
    case EGameModeType::SinglePlayer:
        StartSinglePlayer();
        break;

    case EGameModeType::Coop:
        StartCoop();
        break;
    }
}

void ASnakeGameMode::StartSinglePlayer()
{
    TArray<AActor*> PlayerStarts;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);

    if (PlayerStarts.Num() == 0) return;

    FVector Location = PlayerStarts[0]->GetActorLocation();
    FRotator Rotation = PlayerStarts[0]->GetActorRotation();

    APawn* Snake = GetWorld()->SpawnActor<APawn>(SnakePawnClass, Location, Rotation);

    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (PC && Snake)
    {
        PC->Possess(Snake);
    }
}

void ASnakeGameMode::StartCoop()
{
    TArray<AActor*> PlayerStarts;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);

    if (PlayerStarts.Num() < 2) return;

    // Create second player
    UGameplayStatics::CreatePlayer(GetWorld(), 1, true);

    // Spawn snakes
    APawn* Snake1 = GetWorld()->SpawnActor<APawn>(
        SnakePawnClass,
        PlayerStarts[0]->GetActorLocation(),
        PlayerStarts[0]->GetActorRotation()
    );

    APawn* Snake2 = GetWorld()->SpawnActor<APawn>(
        SnakePawnClass,
        PlayerStarts[1]->GetActorLocation(),
        PlayerStarts[1]->GetActorRotation()
    );

    // Possess
    APlayerController* PC0 = UGameplayStatics::GetPlayerController(this, 0);
    APlayerController* PC1 = UGameplayStatics::GetPlayerController(this, 1);

    if (PC0 && Snake1)
    {
        PC0->Possess(Snake1);
    }

    if (PC1 && Snake2)
    {
        PC1->Possess(Snake2);
    }
}

void ASnakeGameMode::TriggerGameOver()
{
    ASnakeGameState* GS = GetGameState<ASnakeGameState>();
    if (!GS) return;

    GS->SetGameOver(true);

    // Pause the game
    UGameplayStatics::SetGamePaused(this, true);

    // Show mouse for UI
    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (PC)
    {
        PC->bShowMouseCursor = true;
        PC->SetInputMode(FInputModeUIOnly());
    }

    // OPTIONAL: Spawn Game Over UI (if you have one)
}
