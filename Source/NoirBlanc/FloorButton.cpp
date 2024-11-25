// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorButton.h"

#include "BishopGameMode.h"
#include "Components/BoxComponent.h"

AFloorButton::AFloorButton()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	RootComponent = BoxComponent;

	ButtonBodyMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonBodyMesh"));
	ButtonBodyMeshComponent->SetupAttachment(BoxComponent);

	ButtonSwitchMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonSwitchMesh"));
	ButtonSwitchMeshComponent->SetupAttachment(BoxComponent);

	DetectAreaBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("DetectArea"));
	DetectAreaBoxComponent->SetupAttachment(ButtonSwitchMeshComponent);
}

void AFloorButton::BeginPlay()
{
	Super::BeginPlay();

	DetectAreaBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AFloorButton::OnDetectAreaBeginOverlap);
	DetectAreaBoxComponent->OnComponentEndOverlap.AddDynamic(this, &AFloorButton::OnDetectAreaEndOverlap);

	OriginalSwitchLocation = ButtonSwitchMeshComponent->GetComponentLocation();
}

void AFloorButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsPressing)
	{
		if (ButtonSwitchMeshComponent->GetComponentLocation().Z >= OriginalSwitchLocation.Z - PressDepth)
		{
			ButtonSwitchMeshComponent->AddWorldOffset(FVector(0, 0, -(PressSpeed * DeltaTime)));
		}
	}
	else
	{
		if (ButtonSwitchMeshComponent->GetComponentLocation().Z < OriginalSwitchLocation.Z)
		{
			ButtonSwitchMeshComponent->AddWorldOffset(FVector(0, 0, PressSpeed * DeltaTime));
		}
	}
}

void AFloorButton::OnDetectAreaBeginOverlap
(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	UE_LOG(LogTemp, Warning, TEXT("Button Begin Overlapped! : %s"), *OtherActor->GetActorNameOrLabel());
	bIsPressing = true;
	ServerRPC_OnButtonPressed();
}

void AFloorButton::OnDetectAreaEndOverlap
(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex
)
{
	UE_LOG(LogTemp, Warning, TEXT("Button End Overlapped! : %s"), *OtherActor->GetActorNameOrLabel());
	bIsPressing = false;
}

void AFloorButton::ServerRPC_OnButtonPressed_Implementation()
{
	ABishopGameMode* BishopGameMode = Cast<ABishopGameMode>(GetWorld()->GetAuthGameMode());
	BishopGameMode->OnButtonPressed();
}
