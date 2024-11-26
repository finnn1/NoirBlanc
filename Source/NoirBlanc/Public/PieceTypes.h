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
 White
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

class NOIRBLANC_API PieceTypes
{

};
