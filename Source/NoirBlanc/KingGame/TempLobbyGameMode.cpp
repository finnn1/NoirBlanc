// Fill out your copyright notice in the Description page of Project Settings.


#include "NoirBlanc/KingGame/TempLobbyGameMode.h"
#include "TempLobbyUI.h"

void ATempLobbyGameMode::BeginPlay()
{
	Super::BeginPlay();

	UTempLobbyUI* TempLobby = CreateWidget<UTempLobbyUI>(GetWorld(), TempLobbyClass);
	if (TempLobby)
	{
		TempLobby->AddToViewport();
	}

	APlayerController* _PlayerController = GetWorld()->GetFirstPlayerController();
	if (_PlayerController)
	{
		_PlayerController->SetShowMouseCursor(true);
	}
}
