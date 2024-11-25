// Fill out your copyright notice in the Description page of Project Settings.


#include "Fortress/Projectile.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Fortress/Cannon.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Grenade = CreateDefaultSubobject<UStaticMeshComponent>("ProjectileMesh");
	RootComponent = Grenade;

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	ProjectileMovementComp->InitialSpeed = LaunchSpeed;
	ProjectileMovementComp->MaxSpeed = 1000.f;
	ProjectileMovementComp->ProjectileGravityScale = 1.0f;

	WindForce = FVector(10.0f, 0.0f, 0.0f);
	WindResistance = 0.1f;

	LaunchSpeed = 1000.0f;
	LaunchDirection = FVector(1.0f, 0.0f, 0.0f);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	ACannon* OwnerCannon = Cast<ACannon>(GetOwner());
	if (OwnerCannon)
	{
		LaunchDirection =
			OwnerCannon->SpawnLocation->GetComponentLocation() - OwnerCannon->SpawnOrigin->GetComponentLocation();
		LaunchDirection.Normalize();
	}
	
	// should be called after beginplay bc of the spawned actors
	ProjectileMovementComp->Velocity = LaunchDirection * LaunchSpeed;
	
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *LaunchDirection.ToString());
	// if (GetOwner()!=nullptr)
	//  	UE_LOG(LogTemp, Warning, TEXT("%s"), *GetOwner()->GetName());
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// UE_LOG(LogTemp, Warning, TEXT("%s"), *GetActorLocation().ToString());
	// UE_LOG(LogTemp, Warning, TEXT("Projectile Velocity: %s"), *ProjectileMovementComp->Velocity.ToString());
}

