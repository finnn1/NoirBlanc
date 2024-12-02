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

void APawnCardGameMode::SetInitCardSetting()
{
	OnShuffleStart.Broadcast();

	for(int i = 0; i < PawnCards.Num(); i++)
	{
		PawnCards[i]->SetActorRotation(FRotator(0));
	}
	
	// 카드 셔플
	GetWorldTimerManager().SetTimer(TimerHandle, [this]() 
	{
		static float ElapsedTime = 0.0f;
		
		if(ElapsedTime >= 5.f)
		{
			InitPawnCardGame();
			ElapsedTime = 0.0f;
			return;
		}
		
		this->CardSpawner->ShuffleCard();
		ElapsedTime += 1.f;
		
	}, 1.f, true);
}

void APawnCardGameMode::InitPawnCardGame()
{
	OnShuffleEnd.Broadcast();
	
	GetWorldTimerManager().ClearTimer(TimerHandle);
	
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

void APawnCardGameMode::StartPost()
{
	OnGameStart.Broadcast();
	FTimerHandle ShuffleHandle;
	GetWorldTimerManager().SetTimer(ShuffleHandle, this, &APawnCardGameMode::SetInitCardSetting, 2.f, false);
}

// TODO Spawner의 변수를 바로 Remove 안 하게 수정
void APawnCardGameMode::DeleteCardFromMap(APawnCard* SelectedCard)
{
	if(CardSpawner)
	{
		CardSpawner->MapUseStr.Remove(SelectedCard);
	}
}

void APawnCardGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	APawnCardController* CardController = Cast<APawnCardController>(NewPlayer);
	if(CardController)
	{
		CurrentPlayerNum++;

		if(PlayerNum <= CurrentPlayerNum)
		{
			FTimerHandle StartHandle;
			GetWorldTimerManager().SetTimer(StartHandle, this, &APawnCardGameMode::StartPost, 2.0f, false);
		}
	}
	
}
