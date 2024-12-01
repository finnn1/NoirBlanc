// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnCardGameMode.h"
#include "NetworkPawn.h"
#include "PawnCardSpawner.h"
#include "PawnCard.h"
#include "PawnCardController.h"
#include "PawnCardDataAsset.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"

void APawnCardGameMode::BeginPlay()
{
	Super::BeginPlay();

	if(TSubCardSpawner)
	{
		CardSpawner = GetWorld()->SpawnActor<APawnCardSpawner>(TSubCardSpawner, FVector(0), FRotator(0));
		if(CardSpawner)
		{
			CardSpawner->InitCardsArray();

			//World의 PawnCard를 배열에 추가
			TArray<AActor*> AllActors;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), APawnCard::StaticClass(), AllActors);
	
			for(AActor* CardActor : AllActors)
			{
				APawnCard* PawnCard = Cast<APawnCard>(CardActor);
				if(PawnCard)
				{
					PawnCards.Add(PawnCard);
				}
			}
		}
	}
}

void APawnCardGameMode::SetInitCardSetting(APawnCardSpawner* Spawner)
{
	// 시작하고 텀
	GetWorldTimerManager().SetTimer(TimerHandle, [this, Spawner]() 
	{
		CardSpawner->ShuffleCard();
			
		static float ElapsedTime = 0.0f;
		ElapsedTime += 1.f;
			
		if(ElapsedTime >= 8.5f)
		{
			InitPawnCardGame();
			ElapsedTime = 0.0f;
		}
	}, 1.f, true);
}

void APawnCardGameMode::InitPawnCardGame()
{
	/*for(int32 i=0; i < PawnCards.Num(); i++)
	{
		PawnCards[i]->SetActorRotation(FRotator(0,180,0));
		PawnCards[i]->ChangeFrontBackState();
	}*/
	GetWorldTimerManager().ClearTimer(TimerHandle);

	OnGameStart.Broadcast();
	
	Players[0]->SetIsTurnPlayer(true);
	OnChangePlayerTurn.Execute(Players[0]);
}

void APawnCardGameMode::AddPlayer(ANetworkPawn* Player)
{
	Players.Add(Player);
}

bool APawnCardGameMode::CheckRemainCards()
{
	int32 NoLuckCardNum = 0;
	PawnCards.RemoveAll([&NoLuckCardNum](APawnCard* Card)
	{
		if(IsValid(Card) && Card->PawnCardData->PawnCardType == PawnCardType::NoLuck)
		{
			NoLuckCardNum++;
		}

		return !IsValid(Card);
	});

	int32 RemainCardNum = PawnCards.Num();
	if(RemainCardNum <= 1)
	{
		GameSet();
	}
	else if(RemainCardNum <= 4 && NoLuckCardNum >= 2)
	{
		GameSet();
	}
	
	return (PawnCards.Num() == 0);
}

void APawnCardGameMode::GameSet()
{
	ANetworkPawn* WinnerPlayer = nullptr;
	int32 HighestScore = 0;
	
	for(ANetworkPawn* Player : Players)
	{
		if(HighestScore < Player->GetPlayerState()->GetScore())
		{
			WinnerPlayer = Player;
			HighestScore = Player->GetPlayerState()->GetScore();
		}
	}
	OnGameSet.ExecuteIfBound(WinnerPlayer);
}

void APawnCardGameMode::ChangeTurn(ANetworkPawn* EndPlayer)
{
	// 이전 플레이어 TurnOwner false 처리
	EndPlayer->SetIsTurnPlayer(false);

	// 다음 플레이어 TurnOwner true 처리
	TurnPlayerIdx = (TurnPlayerIdx + 1) % Players.Num();
	Players[TurnPlayerIdx]->SetIsTurnPlayer(true);

	// Turn Start UI 표시 Delegate
	OnChangePlayerTurn.Execute(Players[TurnPlayerIdx]);
}

void APawnCardGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	CurrentPlayerNum++;

	if(PlayerNum >= CurrentPlayerNum)
	{
		SetInitCardSetting(CardSpawner);
	}
}
