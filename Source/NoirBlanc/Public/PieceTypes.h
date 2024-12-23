// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EPieceType : uint8
{
 Blank,
 King,
 Queen,
 Knight,
 Bishop,
 Rook,
 Pawn
};

UENUM(BlueprintType)
enum class EPieceColor : uint8
{
 Black,
 White,
 Blank
};

inline TArray<TArray<int32>> KnightValue = {
 {2,1},
 {2, -1},
 {-2, 1},
 {-2, -1},
 {1, 2},
 {1, -2},
 {-1, 2},
 {-1, -2},
};

inline TArray<TArray<int32>> KingValue = {
 {1, 1},
 {1, 0},
 {1, -1},
 {0, 1},
 {0, -1},
 {-1, 1},
 {-1, 0},
 {-1, -1},
};

inline TArray<EPieceType> InitTypeValue = {
 EPieceType::Rook, EPieceType::Knight,EPieceType::Bishop, EPieceType::Queen, EPieceType::King, EPieceType::Bishop, EPieceType::Knight, EPieceType::Rook,
 EPieceType::Pawn, EPieceType::Pawn, EPieceType::Pawn, EPieceType::Pawn, EPieceType::Pawn, EPieceType::Pawn, EPieceType::Pawn, EPieceType::Pawn,
 EPieceType::Blank,  EPieceType::Blank,  EPieceType::Blank,  EPieceType::Blank,  EPieceType::Blank,  EPieceType::Blank,  EPieceType::Blank,  EPieceType::Blank,
 EPieceType::Blank,  EPieceType::Blank,  EPieceType::Blank,  EPieceType::Blank,  EPieceType::Blank,  EPieceType::Blank, EPieceType::Blank, EPieceType::Blank,
 EPieceType::Blank, EPieceType::Blank, EPieceType::Blank, EPieceType::Blank, EPieceType::Blank, EPieceType::Blank, EPieceType::Blank, EPieceType::Blank,
 EPieceType::Blank, EPieceType::Blank, EPieceType::Blank, EPieceType::Blank, EPieceType::Blank, EPieceType::Blank, EPieceType::Blank, EPieceType::Blank,
 EPieceType::Pawn, EPieceType::Pawn, EPieceType::Pawn, EPieceType::Pawn, EPieceType::Pawn, EPieceType::Pawn, EPieceType::Pawn, EPieceType::Pawn,
 EPieceType::Rook, EPieceType::Knight,EPieceType::Bishop, EPieceType::Queen, EPieceType::King, EPieceType::Bishop, EPieceType::Knight, EPieceType::Rook
};

inline TArray<EPieceColor> InitColorValue = {
 EPieceColor::Black,  EPieceColor::Black,  EPieceColor::Black, EPieceColor::Black, EPieceColor::Black, EPieceColor::Black, EPieceColor::Black, EPieceColor::Black,
 EPieceColor::Black,  EPieceColor::Black,  EPieceColor::Black, EPieceColor::Black, EPieceColor::Black, EPieceColor::Black, EPieceColor::Black, EPieceColor::Black,
 EPieceColor::Blank,EPieceColor::Blank,EPieceColor::Blank,EPieceColor::Blank,EPieceColor::Blank,EPieceColor::Blank,EPieceColor::Blank,EPieceColor::Blank,
 EPieceColor::Blank,EPieceColor::Blank,EPieceColor::Blank,EPieceColor::Blank,EPieceColor::Blank,EPieceColor::Blank,EPieceColor::Blank,EPieceColor::Blank,
 EPieceColor::Blank,EPieceColor::Blank,EPieceColor::Blank,EPieceColor::Blank,EPieceColor::Blank,EPieceColor::Blank,EPieceColor::Blank,EPieceColor::Blank,
 EPieceColor::Blank,EPieceColor::Blank,EPieceColor::Blank,EPieceColor::Blank,EPieceColor::Blank,EPieceColor::Blank,EPieceColor::Blank,EPieceColor::Blank,
 EPieceColor::White,EPieceColor::White,EPieceColor::White,EPieceColor::White,EPieceColor::White,EPieceColor::White,EPieceColor::White,EPieceColor::White,
 EPieceColor::White,EPieceColor::White,EPieceColor::White,EPieceColor::White,EPieceColor::White,EPieceColor::White,EPieceColor::White,EPieceColor::White

 };

static const TMap<FName, FString> GameLevelMap = {
 {TEXT("King"), TEXT("/Game/Level/Lv_King")},
 {TEXT("Queen"), TEXT("/Game/Level/Lv_King")},
 {TEXT("Rook"), TEXT("/Game/Level/Lv_Fortress")},
 {TEXT("Bishop"), TEXT("/Game/Level/Lv_Bishop")},
 {TEXT("Knight"), TEXT("/Game/Level/Lv_Knight")},
 {TEXT("Pawn"), TEXT("/Game/Level/Lv_PawnCard")},
 {TEXT("Chess"), TEXT("/Game/Level/Lv_ChessBoard")}
};

class NOIRBLANC_API PieceTypes
{

};
