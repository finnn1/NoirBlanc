// Fill out your copyright notice in the Description page of Project Settings.


#include "BishopWeapon.h"

#include "BishopGameMode.h"
#include "TaggerCharacter.h"
#include "Components/BoxComponent.h"
#include "GameFramework/PawnMovementComponent.h"

class ABishopGameMode;

ABishopWeapon::ABishopWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = BoxComponent;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(BoxComponent);
}

void ABishopWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		SetReplicates(true);
	}

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ABishopWeapon::OnBoxComponentOverlap);
}

void ABishopWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// FRotator CurrentRotation = GetActorRotation();
	// if (CurrentRotation.Pitch >= 80.0f)
	// {
	// 	Destroy();
	// 	return;
	// }
	//
	// // 회전
	// float PitchMoveValue = AttackSpeed * DeltaTime;
	// AddActorWorldRotation(FRotator(-PitchMoveValue, 0, 0));

	FVector CurrentLocation = GetActorLocation();
	// if (CurrentLocation.Pitch >= 80.0f)
	// {
	// 	Destroy();
	// 	return;
	// }

	// 전진
	float LocationMoveValue = AttackSpeed * DeltaTime;
	AddActorWorldOffset(FVector(-AttackSpeed, 0, 0));
}

void ABishopWeapon::OnBoxComponentOverlap
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
		ABishopGameMode* _BishopGameMode = Cast<ABishopGameMode>(GetWorld()->GetAuthGameMode());
		if (_BishopGameMode)
		{
			_BishopGameMode->OnTaggerOverlapped(OtherActor);
		}
	}
}
