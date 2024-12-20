// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniGameToChessUI.h"
#include "Components/TextBlock.h"

void UMiniGameToChessUI::UpdateMiniGameToChessUI(EPieceColor LoserColor, EPieceType DeletePieceType)
{
	if(LoserColor == EPieceColor::White)
	{
		Txt_LoseTeam->SetText(FText::FromString(TEXT("블랑")));
	}
	else
	{
		Txt_LoseTeam->SetText(FText::FromString(TEXT("느와르")));
	}
	
	switch (DeletePieceType)
	{
	case EPieceType::Pawn:
		{
			Txt_LosePiece->SetText(FText::FromString(TEXT("폰")));
			break;
		}
	case EPieceType::Knight:
		{
			Txt_LosePiece->SetText(FText::FromString(TEXT("나이트")));
			break;
		}
	case EPieceType::Rook:
		{
			Txt_LosePiece->SetText(FText::FromString(TEXT("룩")));
			break;
		}
	case EPieceType::Bishop:
		{
			Txt_LosePiece->SetText(FText::FromString(TEXT("비숍")));
			break;
		}
	case EPieceType::Queen:
		{
			Txt_LosePiece->SetText(FText::FromString(TEXT("퀸")));
			break;
		}
	case EPieceType::King:
		{
			Txt_LosePiece->SetText(FText::FromString(TEXT("킹")));
			break;
		}
	default:
		{
			break;
		}
	}

	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, this, &UMiniGameToChessUI::DestroyUI, 1, false);
}

void UMiniGameToChessUI::DestroyUI()
{
	RemoveFromParent();
}
