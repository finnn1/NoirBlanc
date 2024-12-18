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
	
// 	// 클라이언트 대기 상태를 체크하는 함수
// 	void CheckPlayersReady();
//
protected:
	UPROPERTY()
	int32 PlayerNum = 2;

	UPROPERTY()
	int32 CurrentPlayerNum = 0;

	UPROPERTY(VisibleAnywhere)
	TArray<class AChessPlayerPawn*> Players;
	
	UPROPERTY(VisibleAnywhere)
	TArray<class AChessPlayerController*> Controllers;
public:
	void AddPlayer(AChessPlayerPawn* Player);
	// 게임 시작 여부를 알리는 함수
	void StartGameWhenReady();

	TArray<AChessPlayerController*> GetControllers();
};
