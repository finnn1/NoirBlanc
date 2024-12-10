// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ChessGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class NOIRBLANC_API AChessGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AChessGameModeBase();
	
// 	// 게임 시작 조건을 체크하는 함수
// 	virtual void StartPlay() override;
//
// protected:
// 	// 현재 접속한 플레이어 수
// 	int32 ConnectedPlayers;
//
// 	// 최대 플레이어 수 (예: 2명 필요)
// 	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game Rules")
// 	int32 RequiredPlayers = 2;
//
// 	// 클라이언트 대기 상태를 체크하는 함수
// 	void CheckPlayersReady();
//
// 	// 게임 시작 여부를 알리는 함수
// 	void StartGameWhenReady();
};
