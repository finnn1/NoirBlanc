// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniGameToChessUI.h"
#include "Components/TextBlock.h"

void UMiniGameToChessUI::UpdateMiniGameToChessUI(FText loseTeam, FText losePiece)
{
	Txt_LoseTeam->SetText(loseTeam);
	Txt_LosePiece->SetText(losePiece);

	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, this, &UMiniGameToChessUI::DestroyUI, 1, false);
	
}

void UMiniGameToChessUI::DestroyUI()
{
	RemoveFromParent();
}
