// Fill out your copyright notice in the Description page of Project Settings.
#include "ChessPiece.h"

// Sets default values
AChessPiece::AChessPiece()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CompMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CompMesh"));
	RootComponent = CompMesh;
}
// Called when the game starts or when spawned
void AChessPiece::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle,this, &AChessPiece::SetPieceMesh, 1.f, false);
}

EPieceType AChessPiece::GetPieceType()
{
	return PieceType;
}

EPieceColor AChessPiece::GetPieceColor()
{
	return PieceColor;
}

void AChessPiece::SetPieceType(EPieceType Type)
{
	PieceType = Type;
}

void AChessPiece::SetPieceColor(EPieceColor Color)
{
	PieceColor = Color;
}

void AChessPiece::SetPieceMesh()
{
	switch(PieceType)
	{
	case EPieceType::Pawn:
		if(PieceColor == EPieceColor::Black)
		{
			CompMesh->SetStaticMesh(Meshes[0]);
		}
		else
		{
			CompMesh->SetStaticMesh(Meshes[1]);
		}
		break;
	case EPieceType::Knight:
		if(PieceColor == EPieceColor::Black)
		{
			CompMesh->SetStaticMesh(Meshes[2]);
		}
		else
		{
			CompMesh->SetStaticMesh(Meshes[3]);
		}
		break;
	case EPieceType::Bishop:
		if(PieceColor == EPieceColor::Black)
		{
			CompMesh->SetStaticMesh(Meshes[4]);
		}
		else
		{
			CompMesh->SetStaticMesh(Meshes[5]);
		}
		break;
	case EPieceType::Rook:
		if(PieceColor == EPieceColor::Black)
		{
			CompMesh->SetStaticMesh(Meshes[6]);
			GEngine->AddOnScreenDebugMessage(1, 3, FColor::Red, "Check");
		}
		else
		{
			CompMesh->SetStaticMesh(Meshes[7]);
		}
		break;
	case EPieceType::Queen:
		if(PieceColor == EPieceColor::Black)
		{
			CompMesh->SetStaticMesh(Meshes[8]);
		}
		else
		{
			CompMesh->SetStaticMesh(Meshes[9]);
		}
		break;
	case EPieceType::King:
		if(PieceColor == EPieceColor::Black)
		{
			CompMesh->SetStaticMesh(Meshes[10]);
		}
		else
		{
			CompMesh->SetStaticMesh(Meshes[11]);
		}
		break;
	}
}
