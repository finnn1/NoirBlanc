// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnCardGameMode.h"
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
	//World의 PawnCard를 배열에 추가
	TArray<AActor*> AllActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APawnCard::StaticClass(), AllActors);
	
	/*for(AActor* CardActor : AllActors)
	{
		APawnCard* PawnCard = Cast<APawnCard>(CardActor);
		if(PawnCard)
		{
			PawnCards.Add(PawnCard);
		}
	}*/

	//턴 플레이어 시작 
	TurnPlayer = Cast<APawnCardController>(GetWorld()->GetFirstPlayerController());
	
	if(TurnPlayer)
	{
		TurnStart();
	}
}

void APawnCardGameMode::AddPlayerController(APawnCardController* Player)
{
	Players.Add(Player);
}

void APawnCardGameMode::TurnStart()
{
	UE_LOG(LogTemp, Warning, TEXT("Start : Player num is %d"), Players.Num());
	TurnPlayer->SetTurnOwner(true);
	OnTurnStart.Broadcast();
}

void APawnCardGameMode::TurnEnd(APawnCardController* EndPlayer)
{
	/*for(APawnCard* PawnCard : PawnCards)
	{
		if(PawnCard->FrontBackState == CardState::Front)
		{
			UE_LOG(LogTemp, Warning, TEXT("Front Card is %s"), *PawnCard->GetName());
		}
	}*/
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
	TurnPlayerIdx = (TurnPlayerIdx + 1) % Players.Num();
	TurnPlayer = Players[TurnPlayerIdx];
	UE_LOG(LogTemp, Warning, TEXT("Player num is %d"), Players.Num());
	UE_LOG(LogTemp, Warning, TEXT("Turn Start is %s"), *TurnPlayer->GetName());
	TurnStart();
}

void APawnCardGameMode::GameEnd()
{
}
