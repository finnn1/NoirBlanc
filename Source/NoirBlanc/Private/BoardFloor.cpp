// Fill out your copyright notice in the Description page of Project Settings.


#include "BoardFloor.h"

// Sets default values
ABoardFloor::ABoardFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABoardFloor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABoardFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

AChessPiece* ABoardFloor::GetPieceOnFloor()
{
	return PieceOnFloor;
}

void ABoardFloor::SetPieceOnFloor(AChessPiece* Piece)
{
	PieceOnFloor = Piece;
}

int32 ABoardFloor::GetRow()
{
	return FloorRow;
}

int32 ABoardFloor::GetCol()
{
	return FloorCol;
}

void ABoardFloor::SetRow(int32 Row)
{
	FloorRow = Row;
}

void ABoardFloor::SetCol(int32 Col)
{
	FloorCol = Col;
}