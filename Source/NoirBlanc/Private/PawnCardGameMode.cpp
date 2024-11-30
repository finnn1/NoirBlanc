// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnCardGameMode.h"
#include "NetworkPawn.h"
#include "PawnCardSpawner.h"
#include "PawnCard.h"
#include "PawnCardController.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"

void APawnCardGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	if(TSubCardSpawner)
	{
		APawnCardSpawner* CardSpawner = GetWorld()->SpawnActor<APawnCardSpawner>(TSubCardSpawner, FVector(0), FRotator(0));
		CardSpawner->ShuffleCards();

		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([CardSpawner]() {
			CardSpawner->ReShuffleCard();
		}), 2.0f, true, 2.f);

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

void APawnCardGameMode::InitPawnCardGame()
{
	//TODO : 게임 시작 UI
	
	//턴 플레이어 시작
	/*APawn* TurnPlayerPawn = Cast<APawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
	
	if(TurnPlayerPawn)
	{
		TurnNetPlayer = Cast<ANetworkPawn>(TurnPlayerPawn);
	}*/
}

void APawnCardGameMode::AddPlayer(ANetworkPawn* Player)
{
	Players.Add(Player);
}

bool APawnCardGameMode::CheckRemainCards()
{
	PawnCards.RemoveAll([](APawnCard* Card)
	{
		return !IsValid(Card);
	});

	if(PawnCards.Num() == 0)
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
