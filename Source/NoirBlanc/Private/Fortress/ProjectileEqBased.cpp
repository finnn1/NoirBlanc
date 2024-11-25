// Fill out your copyright notice in the Description page of Project Settings.


#include "Fortress/ProjectileEqBased.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Fortress/Cannon.h"
#include "Kismet/GameplayStatics.h"

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
	InitSpeed = 0.0f;
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

		InitSpeed = OwnerCannon->ProjectileVelocity;
		InitVelocity = LaunchDirection.GetSafeNormal() * InitSpeed;
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red,   FString::Printf(TEXT("Speed: %f"), InitSpeed));
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red,   FString::Printf(TEXT("Velocity: %s"), *InitVelocity.ToString()));

		/* if simulate physics is false*/
		float Mass = 1;
		// impulse = F*t or m*v, bVelChange: to consider mass or not: engine automatically set the mass
		Mesh->AddImpulse(InitVelocity* Mass, NAME_None, true);

		// when collision happens
		Mesh->IgnoreActorWhenMoving(this->Owner, true);
		//Mesh->OnComponentBeginOverlap.AddDynamic(this, &AProjectileEqBased::OnProjectileOverlap);
		Mesh->OnComponentHit.AddDynamic(this, &AProjectileEqBased::OnProjectileHit);
		
	}
	
}

// Called every frame
void AProjectileEqBased::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ElapsedTime += DeltaTime;
	
	FVector NewLocation = CurrLocation;

	/* if simulate physics is true*/
	// NewLocation.X += InitVelocity.X * DeltaTime;
	// NewLocation.Y += InitVelocity.Y * DeltaTime;
	// NewLocation.Z += InitVelocity.Z * DeltaTime - 0.5f*Gravity*FMath::Square(DeltaTime);
	//
	// SetActorLocation(NewLocation, true);
	//
	// InitVelocity.Z -= Gravity*DeltaTime;
	
	CurrLocation = NewLocation;
}



void AProjectileEqBased::SetWindResistance(FVector WindDirection, float Resistance)
{
}

void AProjectileEqBased::OnProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		ACannon*  Opponent = Cast<ACannon>(OtherActor);
		if (Opponent == this->Owner)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, TEXT("Owner"));
			return;
		}
		if(Opponent!=nullptr)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, TEXT("Cannon"));
			Opponent->Health -= 10.0f;
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, TEXT("Health: %f"), Opponent->Health);		// error
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, FString::Printf(TEXT("Health: %f"), Opponent->Health));
		}
	}
}



void AProjectileEqBased::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, FVector OtherNormal, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this)
	{
		ACannon*  Opponent = Cast<ACannon>(OtherActor);
		if (Opponent == this->Owner) return;
		if(Opponent)
		{
			Opponent->Health -= 10.0f;
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, FString::Printf(TEXT("Health: %f"), Opponent->Health));

			if (BombEffect)
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BombEffect, Hit.ImpactPoint, FRotator(0, 0, 0));
		}
		Destroy();
	}
}

