// Fill out your copyright notice in the Description page of Project Settings.


#include "Fortress/ProjectileEqBased.h"

#include "Components/HorizontalBox.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextBlock.h"
#include "Fortress/Cannon.h"
#include "Fortress/FortressGameMode.h"
#include "Fortress/FortressPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Fortress/FortressUI.h"
#include "GameFramework/GameMode.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AProjectileEqBased::AProjectileEqBased()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	Gravity = 980.0f; // unit: (cm/s^2)
	ElapsedTime = 0.0f;
	WindResistance = 0.1f;
	Force = 1.0f;
	Speed = 0.0f;
	Mass = 1;

	//bReplicates = true;
}

// Called when the game starts or when spawned
void AProjectileEqBased::BeginPlay()
{
	Super::BeginPlay();

	//SetReplicateMovement(true);

	// this projectile owner != playerOwner
	OwnerCannon = Cast<ACannon>(GetOwner());
	playerCannon = Cast<ACannon>(GetWorld()->GetFirstPlayerController()->GetPawn());
	playerUI = playerCannon->FortressUI;

	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red,
	                                 FString::Printf(TEXT("Owner: %s"), *OwnerCannon->GetName()));

	if (OwnerCannon)
	{
		FVector LaunchDirection = OwnerCannon->SpawnLocation->GetComponentLocation() -
			OwnerCannon->SpawnOrigin->GetComponentLocation();
		// if (Owner->Owner)
		// 	UE_LOG(LogTemp, Warning, TEXT("Owner->Owner: %s"), *Owner->Owner->GetName());
			SetSpeedAddImpuse(LaunchDirection);

		// InitVelocity = LaunchDirection.GetSafeNormal() * Speed;
		//
		// /* if simulate physics is false*/
		// // impulse = F*t or m*v, bVelChange: to consider mass or not: engine automatically set the mass
		// Mesh->AddImpulse(InitVelocity * Mass, NAME_None, true);

		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red,
		                                 FString::Printf(TEXT("Speed: %f"), Speed));
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red,
		                                 FString::Printf(TEXT("Velocity: %s"), *InitVelocity.ToString()));

		// when collision happens
		Mesh->IgnoreActorWhenMoving(OwnerCannon, true);
		Mesh->OnComponentHit.AddDynamic(this, &AProjectileEqBased::OnProjectileHit);
	}
}

void AProjectileEqBased::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AProjectileEqBased, InitVelocity);
}

void AProjectileEqBased::SetSpeedAddImpuse(FVector Direction)
{
	Speed = OwnerCannon->ProjectileVelocity;
	InitVelocity = Direction.GetSafeNormal() * Speed;

	/* if simulate physics is false*/
	// impulse = F*t or m*v, bVelChange: to consider mass or not: engine automatically set the mass
	Mesh->AddImpulse(InitVelocity * Mass, NAME_None, true);
}

void AProjectileEqBased::ServerRPC_SetSpeedAddImpuse_Implementation(FVector Direction)
{
	Speed = OwnerCannon->ProjectileVelocity;
	InitVelocity = Direction.GetSafeNormal() * Speed;

	/* if simulate physics is false*/
	// impulse = F*t or m*v, bVelChange: to consider mass or not: engine automatically set the mass
	Mesh->AddImpulse(InitVelocity * Mass, NAME_None, true);
}


// Called every frame
void AProjectileEqBased::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ElapsedTime += DeltaTime;

	/* if simulate physics is true*/
	// NewLocation.X += InitVelocity.X * DeltaTime;
	// NewLocation.Y += InitVelocity.Y * DeltaTime;
	// NewLocation.Z += InitVelocity.Z * DeltaTime - 0.5f*Gravity*FMath::Square(DeltaTime);
	//
	// SetActorLocation(NewLocation, true);
	//
	// InitVelocity.Z -= Gravity*DeltaTime;
}


void AProjectileEqBased::SetWindResistance(FVector WindDirection, float Resistance)
{
}




// void AProjectileEqBased::OnProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
// 	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
// {
// 	if (OtherActor && OtherActor != this)
// 	{
// 		ACannon*  Opponent = Cast<ACannon>(OtherActor);
// 		if (Opponent == this->Owner)
// 		{
// 			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, TEXT("Owner"));
// 			return;
// 		}
// 		if(Opponent!=nullptr)
// 		{
// 			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, TEXT("Cannon"));
// 			Opponent->Health -= 10.0f;
// 			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, TEXT("Health: %f"), Opponent->Health);		// error
// 			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, FString::Printf(TEXT("Health: %f"), Opponent->Health));
// 		}
// 	}
// }


void AProjectileEqBased::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, FVector OtherNormal, const FHitResult& Hit)
{
	if (OtherActor && OtherActor == this->Owner)
	{
		return;		// if the projectile hit the owner Cannon, return
	}
	
	if (OtherActor != nullptr)
	{
		ACannon* Opponent = Cast<ACannon>(OtherActor);
		if (Opponent)		// it the projectile hit the opponent Cannon
		{
			Opponent->Health -= Opponent->Damage;
			
			playerUI->ApplyDamageHPBar(Opponent, playerCannon);

			// noir-client-black, blanc-server-white
			// whether the opponent is server of client
			if (Opponent->Health <= 0)
			{
				int32 num = 0;
				if (Opponent->HasAuthority())	// server
				{
					if (Opponent->IsLocallyControlled()) num = 1;
					else num = 0;
				}
				else		// client
				{
					if (Opponent->IsLocallyControlled()) num = 0;
					else num = 1;
				}
				playerCannon->FortressUI->GameOver(num);
			}
				
			UE_LOG(LogTemp, Warning, TEXT("%s"), *Opponent->GetName());
			// error message: OwnerCannon->FortressUI == null
		
			if (BombEffect)
				PlayBombEffect(Hit);
		}
		Destroy();
	}
	
	playerUI->SetTurnWidgetVisible();
}

void AProjectileEqBased::SetTurnWidgetHidden()
{
	UE_LOG(LogTemp, Warning, TEXT("Turn WidgetHidden"));
	playerUI->horizontalBox_Turn->SetVisibility(ESlateVisibility::Hidden);
}

void AProjectileEqBased::PlayBombEffect(const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("HitResult: %s"), *Hit.GetActor()->GetActorNameOrLabel());
	//ServerRPC_PlayBombEffect(Hit);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BombEffect, Hit.ImpactPoint, FRotator(0, 0, 0));
}

void AProjectileEqBased::MulticastRPC_PlayBombEffect_Implementation(const FHitResult& Hit)
{
	if (BombEffect)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BombEffect, Hit.ImpactPoint, FRotator(0, 0, 0));
}

void AProjectileEqBased::ServerRPC_PlayBombEffect_Implementation(const FHitResult& Hit)
{
	MulticastRPC_PlayBombEffect(Hit);
}
