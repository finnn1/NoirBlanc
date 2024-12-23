// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleUI.h"

#include "PieceTypes.h"
#include "Components/TextBlock.h"

void UBattleUI::UpdateBattleUI(EPieceType GamePiece,FString QueenLevel, bool bIsKingGame)
{
	switch (GamePiece)
	{
		case EPieceType::Pawn:
			{
				Txt_Piece->SetText(FText::FromString(TEXT("폰의 미니게임!")));
				Txt_Game->SetText(FText::FromString(TEXT("카드 맞추기")));
				break;
			}
		case EPieceType::Knight:
			{
				Txt_Piece->SetText(FText::FromString(TEXT("나이트의 미니게임!")));
				Txt_Game->SetText(FText::FromString(TEXT("경마 레이스")));
				break;
			}
		case EPieceType::Rook:
			{
				Txt_Piece->SetText(FText::FromString(TEXT("룩의 미니게임!")));
				Txt_Game->SetText(FText::FromString(TEXT("포탄 날리기")));
				break;
			}
		case EPieceType::Bishop:
			{
				Txt_Piece->SetText(FText::FromString(TEXT("비숍의 미니게임!")));
				Txt_Game->SetText(FText::FromString(TEXT("그대로 멈춰라")));
				break;
			}
		case EPieceType::Queen:
			{
				Txt_Piece->SetText(FText::FromString(TEXT("퀸의 미니게임!")));
				if(QueenLevel == TEXT("Pawn"))
				{
					Txt_Game->SetText(FText::FromString(TEXT("카드 맞추기")));
				}
				else if(QueenLevel == TEXT("Knight"))
				{
					Txt_Game->SetText(FText::FromString(TEXT("경마 레이스")));
				}
				else if(QueenLevel == TEXT("Rook"))
				{
					Txt_Game->SetText(FText::FromString(TEXT("포탄 날리기")));
				}
				else if(QueenLevel == TEXT("Bishop"))
				{
					Txt_Game->SetText(FText::FromString(TEXT("그대로 멈춰라")));
				}
				break;
			}
		case EPieceType::King:
			{
				// TODO : 킹의 경우 이 UI 대신 스페셜 UI 넣기
				Txt_Piece->SetText(FText::FromString(TEXT("킹의 미니게임!")));
				if(bIsKingGame)
				{
					Txt_Game->SetText(FText::FromString(TEXT("레이저 피하기")));
				}
				else
				{
					if(QueenLevel == TEXT("Pawn"))
					{
						Txt_Game->SetText(FText::FromString(TEXT("카드 맞추기")));
					}
					else if(QueenLevel == TEXT("Knight"))
					{
						Txt_Game->SetText(FText::FromString(TEXT("경마 레이스")));
					}
					else if(QueenLevel == TEXT("Rook"))
					{
						Txt_Game->SetText(FText::FromString(TEXT("포탄 날리기")));
					}
					else if(QueenLevel == TEXT("Bishop"))
					{
						Txt_Game->SetText(FText::FromString(TEXT("그대로 멈춰라")));
					}
				}
				break;
			}
		default:
			{
				break;
			}
	}
}
