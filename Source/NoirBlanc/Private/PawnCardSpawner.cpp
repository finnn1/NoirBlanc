// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnCardSpawner.h"

#include "PawnCard.h"

// Sets default values
APawnCardSpawner::APawnCardSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Timeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));
	StartMoveFloat.BindUFunction(this, FName("StartMoveCard"));
	EndMoveEvent.BindUFunction(this, FName("EndMoveCard")); 
}

// Called when the game starts or when spawned
void APawnCardSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	if (MovingLinearCurve)
	{
		Timeline->AddInterpFloat(MovingLinearCurve, StartMoveFloat);
		Timeline->SetTimelineFinishedFunc(EndMoveEvent);
		
		Timeline->SetLooping(false);
		Timeline->SetTimelineLength(1.0f);
	}
	
	ShuffleCards();
}

// Called every frame
void APawnCardSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APawnCardSpawner::ShuffleCards()
{
	TArray<TSubclassOf<APawnCard>> NewCards = Cards;
	NewCards.Append(Cards);
	
	for(int i = 0; i < NewCards.Num(); i++)
	{
		int32 RandomIndex = FMath::RandRange(0, NewCards.Num() - 1);
		if(i != 0 && i == RandomIndex)
		{
			RandomIndex--;
		}
		NewCards.Swap(i, RandomIndex);
	}

	DistributeCards(NewCards);
}

void APawnCardSpawner::DistributeCards(TArray<TSubclassOf<APawnCard>> ShuffledCards)
{
	FVector CardLocation = FVector(0, 0, 30);
	int32 TileHeight = 0;
	
	if(ShuffledCards.Num() > 0)
	{
		//제곱근하고 올림
		TileHeight = FMath::CeilToInt32(FMath::Sqrt(static_cast<float>(ShuffledCards.Num())));
	}
	
	for(int i = 0; i < ShuffledCards.Num(); i++)
	{
		SpawnedCard = GetWorld()->SpawnActor<APawnCard>(ShuffledCards[i], FTransform());
		if(SpawnedCard)
		{
			if(i != 0 && i % TileHeight == 0)
			{
				CardLocation.Z += CardMarginZ;
				CardLocation.X = 0;
			}

			CardLocationMap.Add(SpawnedCard, CardLocation);
			SpawnedCard->SetActorLocation(CardLocation);

			CardLocation.X += CardMarginX;
		}
	}
}

void APawnCardSpawner::StartMoveCard(float value)
{
	UE_LOG(LogTemp, Warning, TEXT("Lerp"));
	if(SpawnedCard)
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnedCard is %s"), *SpawnedCard->GetName());
		SpawnedCard->SetActorLocation(FMath::Lerp(SpawnedCard->GetActorLocation(), CardLocationMap[SpawnedCard], value));
	}
}

void APawnCardSpawner::EndMoveCard()
{
}

