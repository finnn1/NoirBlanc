// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessPiece.h"

// Sets default values
AChessPiece::AChessPiece()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}
// Called when the game starts or when spawned
void AChessPiece::BeginPlay()
{
	Super::BeginPlay();
}

EPieceType AChessPiece::GetPieceType()
{
	return PieceType;
}

EPieceColor AChessPiece::GetPieceColor()
{
	return PieceColor;
}