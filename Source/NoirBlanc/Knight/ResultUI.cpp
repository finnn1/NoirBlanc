// Fill out your copyright notice in the Description page of Project Settings.


#include "ResultUI.h"

void UResultUI::ShowResult(EPieceType Attacker, EPieceColor AttackColor, EPieceType Defender, EPieceColor DefendColor,
                           EPieceColor Winner)
{
	/*
	if(AttackColor == EPieceColor::Black)
	{
		FSlateBrush noirBrush;
		noirBrush.SetResourceObject(PieceTextures[(int)Attacker]);
		Noir->SetBrush(noirBrush);
		Noir->SetColorAndOpacity(FLinearColor(0, 0, 0, 1));

		FSlateBrush blancBrush;
		blancBrush.SetResourceObject(PieceTextures[(int)Defender]);
		Blanc->SetBrush(blancBrush);
		Blanc->SetColorAndOpacity(FLinearColor(1, 1, 1, 1));
	}
	else
	{
		FSlateBrush noirBrush;
		noirBrush.SetResourceObject(PieceTextures[(int)Attacker]);
		Noir->SetBrush(noirBrush);
		Noir->SetColorAndOpacity(FLinearColor(0, 0, 0, 1));

		FSlateBrush blancBrush;
		blancBrush.SetResourceObject(PieceTextures[(int)Defender]);
		Blanc->SetBrush(blancBrush);
		Blanc->SetColorAndOpacity(FLinearColor(1, 1, 1, 1));
	}
	*/

	PlayAnimation(ResultAnim1);
}
