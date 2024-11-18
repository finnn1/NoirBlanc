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

class NOIRBLANC_API PieceTypes
{

};
