// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnCardGameMode.h"
#include "NetworkPawn.h"
#include "PawnCard.h"
#include "PawnCardController.h"
#include "Kismet/GameplayStatics.h"

void APawnCardGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	//InitPawnCardGame();
}

void APawnCardGameMode::InitPawnCardGame()
{
	//TODO : 게임 시작 UI
	
	//턴 플레이어 시작
	TurnPlayerContr = Cast<APawnCardController>(GetWorld()->GetFirstPlayerController());
	
	if(TurnPlayerContr)
	{
		TurnStart();
	}
}

void APawnCardGameMode::AddPlayer(ANetworkPawn* Player)
{
	Players.Add(Player);
}

void APawnCardGameMode::TurnStart()
{
	UE_LOG(LogTemp, Warning, TEXT("Start : Player num is %d"), Players.Num());
	TurnPlayerContr->SetTurnOwner(true);
	OnTurnStart.Broadcast();
}

void APawnCardGameMode::TurnEnd(APawnCardController* EndPlayer)
{
	EndPlayer->SetTurnOwner(false);
	OnTurnEnd.Broadcast();

	ChangePlayerTurn();
}

void APawnCardGameMode::ChangeScore()
{
	OnChangeScore.Broadcast();
}

void APawnCardGameMode::ChangePlayerTurn()
{
	/*TurnPlayerIdx = (TurnPlayerIdx + 1) % Players.Num();
	TurnPlayer = Players[TurnPlayerIdx];
	TurnStart();*/
}

void APawnCardGameMode::GameEnd()
{
}
