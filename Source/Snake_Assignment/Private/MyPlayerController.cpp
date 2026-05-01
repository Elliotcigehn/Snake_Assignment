// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "GameFramework/Pawn.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "Kismet/GameplayStatics.h"

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (IMC_SnakeDefault)
			{
				Subsystem->AddMappingContext(IMC_SnakeDefault, 0);
			}
		}
	}
}

void AMyPlayerController::SpawnPlayer(const FInputActionValue& Value)
{
	UGameplayStatics::CreatePlayer(GetWorld(), 1, true);
}

void AMyPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn) return;

	if (!FMath::IsNearlyZero(TurnInput))
	{
		FRotator NewRotation = ControlledPawn->GetActorRotation();
		NewRotation.Yaw += TurnInput * turnSpeed * DeltaTime;
		ControlledPawn->SetActorRotation(NewRotation);
	}

	ControlledPawn->AddActorWorldOffset(ControlledPawn->GetActorForwardVector() * movementSpeed * DeltaTime, true);
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		if (IA_Turn) 
		{
			EnhancedInputComponent->BindAction(IA_Turn, ETriggerEvent::Triggered, this, &AMyPlayerController::Turn);
			EnhancedInputComponent->BindAction(IA_Turn, ETriggerEvent::Completed, this, &AMyPlayerController::Turn);
		}

		if (IA_TogglePause)
		{
			EnhancedInputComponent->BindAction(IA_TogglePause, ETriggerEvent::Triggered, this, &AMyPlayerController::TogglePause);
		}

		if (IA_RestartLevel)
		{
			EnhancedInputComponent->BindAction(IA_RestartLevel, ETriggerEvent::Triggered, this, &AMyPlayerController::RestartLevel);
			EnhancedInputComponent->BindAction(IA_RestartLevel, ETriggerEvent::Completed, this, &AMyPlayerController::RestartLevel);
		}
		
		if (IA_SpawnPlayer)
		{
			EnhancedInputComponent->BindAction(IA_SpawnPlayer, ETriggerEvent::Triggered, this, &AMyPlayerController::SpawnPlayer);
			EnhancedInputComponent->BindAction(IA_SpawnPlayer, ETriggerEvent::Completed, this, &AMyPlayerController::SpawnPlayer);
		}
		
	}
}

void AMyPlayerController::Turn(const FInputActionValue& Value)
{
	TurnInput = Value.Get<float>();
}

void AMyPlayerController::TogglePause()
{
	bool bIsPaused = UGameplayStatics::IsGamePaused(this);

	bShowMouseCursor = !bIsPaused;

	if (bIsPaused)
	{
		UGameplayStatics::SetGamePaused(this, false);
		SetInputMode(FInputModeGameOnly());
	}
	else
	{
		UGameplayStatics::SetGamePaused(this, true);
		SetInputMode(FInputModeUIOnly());
	}
}

void AMyPlayerController::RestartLevel()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

