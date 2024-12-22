// Fill out your copyright notice in the Description page of Project Settings.


#include "TravelPlayerController.h"

void ATravelPlayerController::ServerRPC_LevelTravelToChess_Implementation()
{
	GetWorld()->ServerTravel(TEXT("/Game/Level/Lv_ChessBoard?listen"), true);
}

void ATravelPlayerController::ClientRPC_SetViewTarget_Implementation(class AActor* NewViewTarget, FViewTargetTransitionParams TransitionParams)
{
	if (IsLocalController())
	{
		SetViewTarget(NewViewTarget, TransitionParams);
	}
}
