// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeBase_Knight.h"

#include "NoirBlanc/Knight/GameStateBase_Knight.h"

void AGameModeBase_Knight::BeginPlay()
{
	Super::BeginPlay();
	
	GetGameState<AGameStateBase_Knight>()->StartCountDown();
}

/*
void AGameModeBase_Knight::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	connectedClients+=1;
	if(connectedClients == 2)
	{
		GetGameState<AGameStateBase_Knight>()->StartCountDown();
	}
}
*/
