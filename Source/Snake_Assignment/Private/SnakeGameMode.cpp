#include "SnakeGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "SnakeGameInstance.h"
#include "GameFramework/PlayerController.h"
#include "SnakeGameState.h" 

ASnakeGameMode::ASnakeGameMode()
{
    // Set default pawn class to none since we will spawn manually
    DefaultPawnClass = nullptr;
}

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

AActor* ASnakeGameMode::FindPlayerStartByTag(FName Tag)
{
    TArray<AActor*> PlayerStarts;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);

    for (AActor* Start : PlayerStarts)
    {
        if (Start->ActorHasTag(Tag))
        {
            return Start;
        }
    }

    return nullptr;
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
    UGameplayStatics::CreatePlayer(GetWorld(), 1, true);

    AActor* Start1 = FindPlayerStartByTag("P1");
    AActor* Start2 = FindPlayerStartByTag("P2");

    if (!Start1 || !Start2)
    {
        UE_LOG(LogTemp, Warning, TEXT("PlayerStart tags missing!"));
        return;
    }

    APawn* Snake1 = GetWorld()->SpawnActor<APawn>(
        SnakePawnClass,
        Start1->GetActorLocation(),
        Start1->GetActorRotation()
    );
    APawn* Snake2 = GetWorld()->SpawnActor<APawn>(
        SnakePawnClass,
        Start2->GetActorLocation(),
        Start2->GetActorRotation()
    );
    
    APlayerController* PC0 = UGameplayStatics::GetPlayerController(this, 0);
	APlayerController* PC1 = UGameplayStatics::GetPlayerController(this, 1);


    if (PC0 && Snake1) PC0->Possess(Snake1);
    if (PC1 && Snake2) PC1->Possess(Snake2);
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
