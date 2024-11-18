// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessBoard.h"
#include "BoardFloor.h"

// Sets default values
AChessBoard::AChessBoard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PiecesOnBoard.SetNum(Chess_Num * Chess_Num);
	BoardFloors.SetNum(Chess_Num * Chess_Num);

	PiecesOnBoard.Push({EPieceType::Rook,EPieceType::Knight,EPieceType::Bishop,EPieceType::King, EPieceType::Queen, EPieceType:: Bishop, EPieceType::Knight, EPieceType:: Rook});
}

// Called when the game starts or when spawned
void AChessBoard::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChessBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}