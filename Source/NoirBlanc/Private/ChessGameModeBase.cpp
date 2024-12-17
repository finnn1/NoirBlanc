// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessGameModeBase.h"

#include "GameFramework/PlayerController.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "ChessBoard.h"

AChessGameModeBase::AChessGameModeBase()
{
	
}

void AChessGameModeBase::AddPlayer(class AChessPlayerPawn* Player)
{
	Players.Add(Player);
	
	if(PlayerNum <= Players.Num())
	{
		FTimerHandle StartHandle;
		GetWorldTimerManager().SetTimer(StartHandle, this, &AChessGameModeBase::StartGameWhenReady, 2.0, false);
	}
}
void AChessGameModeBase::StartGameWhenReady()
{
	UE_LOG(LogTemp, Warning, TEXT("All players are connected. Starting the game..."));
	AActor* Board = UGameplayStatics::GetActorOfClass(GetWorld(), AChessBoard::StaticClass());
	AChessBoard* ChessBoard = Cast<AChessBoard>(Board);
	ChessBoard->ServerRPC_StartGame();
}

// void AChessGameModeBase::StartPlay()
// {
// 	Super::StartPlay();
//
// 	// 게임 대기 상태 시작
// 	UE_LOG(LogTemp, Warning, TEXT("Waiting for players to join..."));
//
// }
//
// void AChessGameModeBase::CheckPlayersReady()
// {
// 	// 현재 접속한 플레이어 수 확인
// 	ConnectedPlayers = GetNumPlayers(); // 현재 접속한 클라이언트 수
//
// 	UE_LOG(LogTemp, Warning, TEXT("Connected Players: %d / %d"), ConnectedPlayers, RequiredPlayers);
//
// 	if (ConnectedPlayers >= RequiredPlayers)
// 	{
// 		// 조건 만족 시 게임 시작
// 		StartGameWhenReady();
// 	}
// 	else
// 	{
// 		// 조건이 만족되지 않으면 일정 시간 후 다시 체크
// 		GetWorldTimerManager().SetTimerForNextTick([this]() {
// 			CheckPlayersReady();
// 		});
// 	}
// }
//

