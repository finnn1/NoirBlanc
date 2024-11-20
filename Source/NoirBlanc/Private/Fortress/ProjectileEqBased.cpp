// Fill out your copyright notice in the Description page of Project Settings.


#include "Fortress/ProjectileEqBased.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Fortress/Cannon.h"

// Sets default values
AProjectileEqBased::AProjectileEqBased()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh  =  CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	
	Gravity = 980.0f;		// unit: (cm/s^2)
	ElapsedTime = 0.0f;
	WindResistance = 0.1f;
	Force = 1.0f;
	InitSpeed = 300.0f;
	
}

// Called when the game starts or when spawned
void AProjectileEqBased::BeginPlay()
{
	Super::BeginPlay();

	CurrLocation = GetActorLocation();
	ACannon* OwnerCannon = Cast<ACannon>(GetOwner());
	if (OwnerCannon)
	{
		FVector LaunchDirection =
			OwnerCannon->SpawnLocation->GetComponentLocation() - OwnerCannon->SpawnOrigin->GetComponentLocation();
		
		InitVelocity = LaunchDirection.GetSafeNormal() * InitSpeed;
	}
	
}

// Called every frame
void AProjectileEqBased::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ElapsedTime += DeltaTime;
	
	FVector NewLocation = CurrLocation;
	
	NewLocation.X += InitVelocity.X * DeltaTime;
	NewLocation.Y += InitVelocity.Y * DeltaTime;
	NewLocation.Z += InitVelocity.Z * DeltaTime - 0.5f*Gravity*FMath::Square(DeltaTime);

	SetActorLocation(NewLocation);

	InitVelocity.Z -= Gravity*DeltaTime;

	CurrLocation = NewLocation;
}



void AProjectileEqBased::SetWindResistance(FVector WindDirection, float Resistance)
{
}

