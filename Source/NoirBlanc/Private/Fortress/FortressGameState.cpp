// Fill out your copyright notice in the Description page of Project Settings.


#include "Fortress/FortressGameState.h"

#include "Fortress/FortressGameMode.h"
#include "Fortress/FortressUI.h"

void AFortressGameState::NextRound(ACannon* Cannon, UFortressUI* FortressUI)
{
	// next turn
	if (Cannon->HasAuthority()) // server
	{
		if (Cannon->IsLocallyControlled()) FortressUI->playerPieceColor = EPieceColor::Black;	// noir
		else FortressUI->playerPieceColor = EPieceColor::White;	// blanc
	}
	else // client
	{
		if (Cannon->IsLocallyControlled()) FortressUI->playerPieceColor = EPieceColor::White;
		else FortressUI->playerPieceColor = EPieceColor::Black;
	}
	// switch UI into next player
	if (FortressUI->turnUI)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player UI turned"));
		FortressUI->turnUI->ShowTurn(FortressUI->playerPieceColor);
	}
	// set next turn wind
	if (HasAuthority())
	{
		AFortressGameMode* gm = Cast<AFortressGameMode>(GetWorld()->GetAuthGameMode());
		if (gm != nullptr)
		{
			gm->SetWind();
		}
	}
}
