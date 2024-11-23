// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnCardGameMode.h"
#include "PawnCard.h"
#include "PawnCardController.h"
#include "Kismet/GameplayStatics.h"

void APawnCardGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	InitPawnCardGame();
}

void APawnCardGameMode::InitPawnCardGame()
{	
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

	//턴 플레이어 시작 
	TurnPlayer = Cast<APawnCardController>(GetWorld()->GetFirstPlayerController());
	
	if(TurnPlayer)
	{
		TurnStart();
	}
}

void APawnCardGameMode::TurnStart()
{
	TurnPlayer->SetTurnOwner(true);
	OnTurnStart.Broadcast();
}

void APawnCardGameMode::TurnEnd(APawnCardController* EndPlayer)
{
	for(APawnCard* PawnCard : PawnCards)
	{
		if(PawnCard->FrontBackState == CardState::Front)
		{
			UE_LOG(LogTemp, Warning, TEXT("Front Card is %s"), *PawnCard->GetName());
		}
	}
	EndPlayer->SetTurnOwner(false);
	OnTurnEnd.Broadcast();
}

void APawnCardGameMode::ChangePlayerTurn()
{
	//TODO 플레이어 교체
	/*
	 *TurnPlayer =
	 * 
	 */ 
}

void APawnCardGameMode::GameEnd()
{
}
