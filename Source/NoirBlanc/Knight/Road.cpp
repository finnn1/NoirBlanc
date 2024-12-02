// Fill out your copyright notice in the Description page of Project Settings.


#include "Road.h"

#include "Player_Knight.h"
#include "Components/SplineComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARoad::ARoad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SplineComp = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComp"));
}

// Called when the game starts or when spawned
void ARoad::BeginPlay()
{
	Super::BeginPlay();
	
	if(HasAuthority())
	{
		FTimerHandle handle;
		GetWorldTimerManager().SetTimer(handle, this, &ARoad::FindPlayers, 2, false);
	}
}

void ARoad::FindPlayers()
{
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayer_Knight::StaticClass(), actors);
	for (AActor* Actor : actors)
	{
		AllPlayers.Push(Cast<APlayer_Knight>(Actor));
	}
}

// Called every frame
void ARoad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(HasAuthority())
	{
		for(APlayer_Knight* player : AllPlayers)
		{
			player->CurDistance = SplineComp->FindInputKeyClosestToWorldLocation(player->GetActorLocation());
			player->TotalDistance += player->CurDistance - player->PreviousDistance;
			player->PreviousDistance = player->CurDistance;
		}
	}
}



