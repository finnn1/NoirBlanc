// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnCardController.h"
#include "NetworkPawn.h"

APawnCardController::APawnCardController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APawnCardController::BeginPlay()
{
	Super::BeginPlay();

	ANetworkPawn* GetPlayerPawn = Cast<ANetworkPawn>(GetPawn());
	if(PlayerPawn)
	{
		PlayerPawn = GetPlayerPawn;
	}
	
	//마우스 커서 보이게 하기
	bShowMouseCursor = true;
}

void APawnCardController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void APawnCardController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
}

void APawnCardController::SetTurnOwner(bool IsOwner)
{
	IsTurnOwner = IsOwner;
}

bool APawnCardController::GetTurnOwner() const
{
	return IsTurnOwner;
}
