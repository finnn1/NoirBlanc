// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnCardSpawner.h"

#include "PawnCard.h"

// Sets default values
APawnCardSpawner::APawnCardSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void APawnCardSpawner::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APawnCardSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APawnCardSpawner::ShuffleCards()
{
	//등록한 카드 종류x2를 배열에 넣는다 
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
			
			SpawnedCard->SetActorLocation(CardLocation);

			//구조체
			FMyStruct TempStruct;
			TempStruct.DestLocation = CardLocation;
			TempStruct.ElapsedTime = 0.0f;
			TempStruct.Duration = Duration;
			MapUseStr.Add(SpawnedCard, TempStruct);

			CardLocation.X += CardMarginX;

			CardsOnLevel.Add(SpawnedCard);
			
			//MoveTest(SpawnedCard);
		}
	}
}

void APawnCardSpawner::MoveTest(APawnCard* Card)
{
	if(!Card) return;

	// Actor별 타이머 시작
	FTimerHandle NewTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		NewTimerHandle,
		FTimerDelegate::CreateUObject(this, &APawnCardSpawner::UpdateLerp, Card),
		InRate,
		true
	);

	MapUseStr[Card].TimerHandle = NewTimerHandle; // 핸들 저장
}

void APawnCardSpawner::UpdateLerp(APawnCard* Card)
{
	if (!Card) return;

	MapUseStr[Card].ElapsedTime += InRate;
	float Alpha = FMath::Clamp(MapUseStr[Card].ElapsedTime / MapUseStr[Card].Duration, 0.0f, 1.0f);

	// 위치 보간
	FVector NewLocation = FMath::Lerp(Card->GetActorLocation(), MapUseStr[Card].DestLocation, Alpha);
	Card->SetActorLocation(NewLocation);

	// Lerp 종료 확인
	if (Alpha >= 1.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(MapUseStr[Card].TimerHandle);
	}
}

void APawnCardSpawner::ReShuffleCard()
{
	UE_LOG(LogTemp, Warning, TEXT("Start ReShuffle!!!!!!"));
	
	int32 RandomIndex1 = FMath::RandRange(0, CardsOnLevel.Num() - 1);
	int32 RandomIndex2 = FMath::RandRange(0, CardsOnLevel.Num() - 1);
	
	if(RandomIndex1 != 0 && RandomIndex1 == RandomIndex2)
	{
		RandomIndex2--;
	}

	APawnCard* RandomCard1 = CardsOnLevel[RandomIndex1];
	APawnCard* RandomCard2 = CardsOnLevel[RandomIndex2];

	FVector TempLoc = MapUseStr[RandomCard1].DestLocation;
	
	MapUseStr[RandomCard1].DestLocation = MapUseStr[RandomCard2].DestLocation; 
	MapUseStr[RandomCard2].DestLocation = TempLoc;

	MoveTest(RandomCard1);
	MoveTest(RandomCard2);
}

void APawnCardSpawner::EndMoveCard()
{
	
}
