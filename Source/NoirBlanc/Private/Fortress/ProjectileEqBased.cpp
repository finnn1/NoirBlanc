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
	WindResistance = 1.0f;
	Force = 1.0f;
	Impulse = 0.0f;
	Damage = 10.0f;
	//bReplicates = true;
}

// Called when the game starts or when spawned
void AProjectileEqBased::BeginPlay()
{
	Super::BeginPlay();

	// this projectile owner != playerOwner
	OwnerCannon = Cast<ACannon>(GetOwner());
	playerCannon = Cast<ACannon>(GetWorld()->GetFirstPlayerController()->GetPawn());
	playerUI = playerCannon->FortressUI;

	if (OwnerCannon)
	{
		FVector LaunchDirection =
			OwnerCannon->SpawnLocation->GetComponentLocation() - OwnerCannon->SpawnOrigin->GetComponentLocation();

		WindForce = OwnerCannon->WindForce * WindResistance;
		UE_LOG(LogTemp, Warning, TEXT("Windforce: %s"), *WindForce.ToString());
		
		SetSpeedAddImpuse(LaunchDirection);
		
		if (ShortFireSound)
			UGameplayStatics::PlaySoundAtLocation(this, ShortFireSound, OwnerCannon->SpawnLocation->GetComponentLocation());
		
		// when collision happens
		Mesh->IgnoreActorWhenMoving(OwnerCannon, true);
		
		Mesh->OnComponentHit.AddDynamic(this, &AProjectileEqBased::OnProjectileHit);
	}
}

		// InitImpulse = LaunchDirection.GetSafeNormal() * Speed;
		//
		// /* if simulate physics is false*/
		// // impulse = F*t or m*v, bVelChange: to consider mass or not: engine automatically set the mass
		// Mesh->AddImpulse(InitImpulse * Mass, NAME_None, true);

void AProjectileEqBased::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(AProjectileEqBased, InitImpulse);
}

void AProjectileEqBased::SetSpeedAddImpuse(FVector Direction)
{
	Impulse = OwnerCannon->ProjectileImpulse;

	FVector InitImpulse = Direction.GetSafeNormal() * Impulse;

	// float mass = Mesh->GetMass();
	
	//Mesh->AddImpulse(InitImpulse/mass*20.0f, NAME_None, true);

	// the impulse-momentum theorem 
	Mesh->AddImpulse(InitImpulse, NAME_None, true);

	UE_LOG(LogTemp, Warning, TEXT("Mass: %f"), Mesh->GetMass());
}
	/* if simulate physics is false*/
	// impulse = F*t -> m*v, bVelChange: to consider mass or not: engine automatically set the mass

// Called every frame
void AProjectileEqBased::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Mesh->AddForce(WindForce);

	/* if simulate physics is true*/
	// NewLocation.X += InitImpulse.X * DeltaTime;
	// NewLocation.Y += InitImpulse.Y * DeltaTime;
	// NewLocation.Z += InitImpulse.Z * DeltaTime - 0.5f*Gravity*FMath::Square(DeltaTime);
	//
	// SetActorLocation(NewLocation, true);
	//
	// InitImpulse.Z -= Gravity*DeltaTime;
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
		return; // if the projectile hit the owner Cannon, return
	}

	if (OtherActor != nullptr)
	{
		ACannon* Opponent = Cast<ACannon>(OtherActor);
		if (Opponent) // it the projectile hit the opponent Cannon
		{
			Opponent->Health -= Damage;

			playerUI->ApplyDamageHPBar(Opponent, playerCannon);

			// noir-client-black-1, blanc-server-white-0
			// whether the opponent is server of client
			if (Opponent->Health <= 0)
			{
				int32 num = 0;
				if (Opponent->HasAuthority()) // server
				{
					if (Opponent->IsLocallyControlled()) num = 1;	// noir
					else num = 0;	// blanc
				}
				else // client
				{
					if (Opponent->IsLocallyControlled()) num = 0;
					else num = 1;
				}
				playerCannon->FortressUI->GameOver(num);
			}

			// VFX, SFX
			if (BombEffect) PlayBombEffect(Hit);
			if (BitSound) UGameplayStatics::PlaySoundAtLocation(this, BitSound, Hit.Location);
		}
		Destroy();
	}

	
	// next turn
	if (OwnerCannon->HasAuthority()) // server
	{
		if (OwnerCannon->IsLocallyControlled()) playerUI->playerPieceColor = EPieceColor::Black;	// noir
		else playerUI->playerPieceColor = EPieceColor::White;	// blanc
	}
	else // client
	{
		if (OwnerCannon->IsLocallyControlled()) playerUI->playerPieceColor = EPieceColor::White;
		else playerUI->playerPieceColor = EPieceColor::Black;
	}
	// switch UI into next player
	if (playerUI->turnUI)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player UI turned"));
		playerUI->turnUI->ShowTurn(playerUI->playerPieceColor);
	}
	// set next turn wind
	if (HasAuthority())
	{
		AFortressGameMode* gm = Cast<AFortressGameMode>(GetWorld()->GetAuthGameMode());
		if (gm != nullptr)
		{
			gm->SetWind();
		}
	}
	
	// FTimerHandle TimerHandle;
	// GetWorld()->GetTimerManager().SetTimer(
	// 	TimerHandle,
	// 	FTimerDelegate::CreateUObject(playerUI, &UFortressUI::SetTurnWidgetVisible),
	// 	1.0f,
	// 	false);
}

void AProjectileEqBased::SetTurnWidgetHidden()
{
}

// void AProjectileEqBased::PrepareNextTurn()
// {
// 	// next turn
// 	FTimerHandle TimerHandle;
// 	GetWorld()->GetTimerManager().SetTimer(TimerHandle,
// 		FTimerDelegate::CreateUObject(playerUI, &UFortressUI::SetTurnWidgetVisible), 1.0f, false);
// 	playerUI->SetWindBar(WindForce.X/WindForceMax);
// }

// void AProjectileEqBased::ServerRPC_Init_Implementation()
// {
// 	AFortressGameMode* gm = Cast<AFortressGameMode>(GetWorld()->GetAuthGameMode());
// 	if (gm)
// 		WindForceMax = gm->WindMaxStrength;
// }

void AProjectileEqBased::PlayBombEffect(const FHitResult& Hit)
{
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
