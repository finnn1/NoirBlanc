// Fill out your copyright notice in the Description page of Project Settings.

#include "NoirBlanc/BishopGame/Respawner.h"

#include "TaggerCharacter.h"
#include "Components/BoxComponent.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

ARespawner::ARespawner()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);
}

void ARespawner::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ARespawner::OnComponentBeginOverlap);

	// Find TaggerPlayerStart and Cache it.
	TArray<AActor*> _AllPlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), _AllPlayerStarts);
	for (AActor* PlayerStartActor : _AllPlayerStarts)
	{
		APlayerStart* _PlayerStart = Cast<APlayerStart>(PlayerStartActor);
		if (_PlayerStart)
		{
			if (_PlayerStart->PlayerStartTag == TEXT("Tagger"))
			{
				TaggerPlayerStart = _PlayerStart;
			}
		}
	}
}

void ARespawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARespawner::OnComponentBeginOverlap
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
		if (OtherActor->IsA(ATaggerCharacter::StaticClass()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Overlapped Actor : %s"), *OtherActor->GetActorNameOrLabel());
			Cast<UPrimitiveComponent>(OtherActor->GetRootComponent())->SetAllPhysicsLinearVelocity(FVector(0, 0, 0));
			Cast<UPrimitiveComponent>(OtherActor->GetRootComponent())->SetPhysicsAngularVelocityInDegrees(FVector(0, 0, 0));
			OtherActor->SetActorLocation(TaggerPlayerStart->GetActorLocation());
		}
	}
}
