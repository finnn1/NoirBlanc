// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnCardSpawner.h"

#include "PawnCard.h"
#include "PawnCardDataAsset.h"

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

void APawnCardSpawner::InitCardsArray()
{
	//등록한 카드 종류x2를 배열에 넣는다 
	TArray<TSubclassOf<APawnCard>> NewCards;

	for(int i = 0; i < Cards.Num(); i++)
	{
		APawnCard* DefaultObject = Cards[i]->GetDefaultObject<APawnCard>();
		if(DefaultObject)
		{
			int32 LoopNum = DefaultObject->PawnCardData->TotalNum;
			for(int j = 0; j < LoopNum; j++)
			{
				NewCards.Add(Cards[i]);
			}
		}
	}
	
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
		}
	}
}

// 카드 2개 이동
void APawnCardSpawner::ShuffleCard()
{
	for(int i = 0; i < 2; i++)
	{
		int32 RandomIndex1 = FMath::RandRange(0, CardsOnLevel.Num() - 1);
		int32 RandomIndex2 = FMath::RandRange(0, CardsOnLevel.Num() - 1);

		// 랜덤 카드 2장이 같으면 이전 Index의 카드
		if(RandomIndex1 != 0 && RandomIndex1 == RandomIndex2)
		{
			RandomIndex2--;
		}

		// 배열에서 카드 가져오기
		APawnCard* RandomCard1 = CardsOnLevel[RandomIndex1];
		APawnCard* RandomCard2 = CardsOnLevel[RandomIndex2];

		// 도착 Vector Swap
		FVector TempLoc = MapUseStr[RandomCard1].DestLocation;
		MapUseStr[RandomCard1].DestLocation = MapUseStr[RandomCard2].DestLocation; 
		MapUseStr[RandomCard2].DestLocation = TempLoc;

		SetMoveTimer(RandomCard1);
		SetMoveTimer(RandomCard2);
	}
	
}

void APawnCardSpawner::SetMoveTimer(APawnCard* Card)
{
	if(!Card) return;

	// Actor별 타이머 시작
	FTimerHandle NewTimerHandle;
	
	// 핸들 저장
	MapUseStr[Card].TimerHandle = NewTimerHandle;

	// 이동
	GetWorld()->GetTimerManager().SetTimer(
		MapUseStr[Card].TimerHandle,
		FTimerDelegate::CreateUObject(this, &APawnCardSpawner::UpdateLocationLerp, Card),
		InRate,
		true
	);
}

void APawnCardSpawner::UpdateLocationLerp(APawnCard* Card)
{
	if (!Card) return;

	MapUseStr[Card].ElapsedTime += InRate;
	float Alpha = FMath::Clamp(MapUseStr[Card].ElapsedTime / MapUseStr[Card].Duration, 0.0f, 1.0f);

	// 위치 보간
	FVector NewLocation = FMath::Lerp(Card->GetActorLocation(), MapUseStr[Card].DestLocation, Alpha);
	Card->SetActorLocation(NewLocation);
	
	// 근사값이면 Timer 종료 및 초기화
	if(Card->GetActorLocation().Equals(MapUseStr[Card].DestLocation, 5) )
	{
		Card->SetActorLocation(MapUseStr[Card].DestLocation);
		MapUseStr[Card].ElapsedTime = 0;
		GetWorld()->GetTimerManager().ClearTimer(MapUseStr[Card].TimerHandle);
	}
}