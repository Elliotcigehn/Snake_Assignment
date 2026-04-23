// Fill out your copyright notice in the Description page of Project Settings.


#include "TeleporterActor.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"

// Sets default values
ATeleporterActor::ATeleporterActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;

	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerBox->SetCollisionObjectType(ECC_WorldDynamic);
	TriggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriggerBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	TriggerBox->SetGenerateOverlapEvents(true);

}

// Called when the game starts or when spawned
void ATeleporterActor::BeginPlay()
{
	Super::BeginPlay();
	
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ATeleporterActor::OnOverlap);
}

void ATeleporterActor::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlap Triggered"));

	if (!TargetTeleporter || ! OtherActor) return;

	FVector Destination = TargetTeleporter->GetActorLocation();
	APawn* Pawn = Cast<APawn>(OtherActor);
	if (Pawn)
	{
		Pawn->SetActorLocation(TargetTeleporter->GetActorLocation() + FVector(0, 0, 100));
	}
}


