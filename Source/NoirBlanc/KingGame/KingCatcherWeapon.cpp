// Fill out your copyright notice in the Description page of Project Settings.

#include "NoirBlanc/KingGame/KingCatcherWeapon.h"

#include "KingGameMode.h"
#include "Components/BoxComponent.h"

AKingCatcherWeapon::AKingCatcherWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = BoxComponent;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(BoxComponent);
}

void AKingCatcherWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		SetReplicates(true);
	}

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AKingCatcherWeapon::OnBoxComponentOverlap);
}

void AKingCatcherWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector CurrentLocation = GetActorLocation();
	FVector NewLocation = CurrentLocation + (GetActorForwardVector() * AttackSpeed * DeltaTime);
	SetActorLocation(NewLocation);
}

void AKingCatcherWeapon::OnBoxComponentOverlap
(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	if (HasAuthority())
	{
		AKingGameMode* _KingGameMode = Cast<AKingGameMode>(GetWorld()->GetAuthGameMode());
		_KingGameMode->OnKingCharacterOverlapped(OtherActor);
	}
}
