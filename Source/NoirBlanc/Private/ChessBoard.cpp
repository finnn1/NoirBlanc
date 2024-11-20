// Fill out your copyright notice in the Description page of Project Settings.
#include "ChessBoard.h"
#include "BoardFloor.h"
#include "ChessPiece.h"
#include "PhysicsEngine/PhysicsAsset.h"

// Sets default values
AChessBoard::AChessBoard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
	
	BoardFloors.SetNum(Chess_Num * Chess_Num);
}

// Called when the game starts or when spawned
void AChessBoard::BeginPlay()
{
	Super::BeginPlay();
	for(int i = 0; i < Chess_Num; i++)
	{
		for(int j = 0; j < Chess_Num; j++)
		{
			ABoardFloor* TempFloor = SpawnFloor(i, j);
			TempFloor->SetCol(i);
			TempFloor->SetRow(j);
			BoardFloors[i*Chess_Num + j] = TempFloor;
		}
	}
	InitBoard();
}

// Called every frame
void AChessBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

ABoardFloor* AChessBoard::SpawnFloor(int32 col, int32 row)
{
	if (UWorld* World = GetWorld()) 
	{
		FVector SpawnLocation = GetActorLocation() + FVector(col * 475.0f, row * 475.0f,  0.f) - FVector(1675.f, 1370.f ,0);
		FRotator SpawnRotation = FRotator(0.0f, 0.0f, 0.0f);
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		ABoardFloor* SpawnedActor = World->SpawnActor<ABoardFloor>(FloorClass, SpawnLocation, SpawnRotation, SpawnParams);

		SpawnedActor->SetRow(row);
		SpawnedActor->SetCol(col);
		
		if(SpawnedActor)
		{
			return SpawnedActor;
		}
	}
	return nullptr;
}

void AChessBoard::InitBoard()
{
	//Black Side
	//Rook
	InitPiece(0, EPieceType::Rook, EPieceColor::Black);
	//Knight
	InitPiece(1, EPieceType::Knight, EPieceColor::Black);
	//Bishop
	InitPiece(2, EPieceType::Bishop, EPieceColor::Black);
	//Queen
	InitPiece(3, EPieceType::Queen, EPieceColor::Black);
	//King
	InitPiece(4, EPieceType::King, EPieceColor::Black);
	//Bishop
	InitPiece(5, EPieceType::Bishop, EPieceColor::Black);
	//Kinght
	InitPiece(6, EPieceType::Knight, EPieceColor::Black);
	//Rook
	InitPiece(7, EPieceType::Rook, EPieceColor::Black);
}

void AChessBoard::InitPiece(int32 num, EPieceType type, EPieceColor color)
{
	FVector SpawnLocation = BoardFloors[num]->GetActorLocation() + FVector(0.f, 0.f, 180.f);
	FRotator SpawnRotation = FRotator(0.0f, 270.f, 0.0f);
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	AChessPiece* SpawnedPiece = GetWorld()->SpawnActor<AChessPiece>(PieceClass, SpawnLocation, SpawnRotation, SpawnParams);
	SpawnedPiece->SetPieceColor(color);
	SpawnedPiece->SetPieceType(type);
	if (USceneComponent* RootComp = SpawnedPiece->GetRootComponent())
	{
		RootComp->SetWorldScale3D(FVector(100.f, 100.f, 100.f));
	}
	BoardFloors[num]->SetPieceOnFloor(SpawnedPiece);
}