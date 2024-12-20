// Fill out your copyright notice in the Description page of Project Settings.


#include "BoardFloor.h"

#include "DiffUtils.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"
// Sets default values
ABoardFloor::ABoardFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CompGreen = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CompGreen"));
	CompGreen->SetupAttachment(RootComponent);
	RootComponent = CompGreen;
	CompGreen->SetVisibility(false);

	CompBlue = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CompBlue"));
	CompBlue->SetupAttachment(RootComponent);
	CompBlue->SetVisibility(false);

	CompRed = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CompRed"));
	CompRed->SetupAttachment(RootComponent);
	CompRed->SetVisibility(false);
	
	CompHighlight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CompHighlight"));
	CompHighlight->SetupAttachment(RootComponent);
	CompHighlight->SetVisibility(false);
}

// Called when the game starts or when spawned
void ABoardFloor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABoardFloor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABoardFloor, PieceOnFloor);
	DOREPLIFETIME(ABoardFloor, FloorRow);
	DOREPLIFETIME(ABoardFloor, FloorCol);
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

void ABoardFloor::ToggleGreen()
{
	bool bCurrentVisibility = CompGreen->IsVisible();
	CompGreen->SetVisibility(!bCurrentVisibility);
}

void ABoardFloor::ToggleRed()
{
	bool bCurrentVisibility = CompRed->IsVisible();
	CompRed->SetVisibility(!bCurrentVisibility);
}

void ABoardFloor::ToggleBlue()
{
	bool bCurrentVisibility = CompBlue->IsVisible();
	CompBlue->SetVisibility(!bCurrentVisibility);
}

void ABoardFloor::ActivateGreen()
{
	CompGreen->SetVisibility(true);
}

void ABoardFloor::DeactivateGreen()
{
	CompGreen->SetVisibility(false);
}

void ABoardFloor::ActivateRed()
{
	CompRed->SetVisibility(true);
}

void ABoardFloor::DeactivateRed()
{
	CompRed->SetVisibility(false);
}

void ABoardFloor::ActivateBlue()
{
	CompBlue->SetVisibility(true);
}

void ABoardFloor::DeactivateBlue()
{
	CompBlue->SetVisibility(false);
}

void ABoardFloor::ActivateHighlight()
{
	CompHighlight->SetVisibility(true);
}

void ABoardFloor::DeactivateHighlight()
{
	CompHighlight->SetVisibility(false);
}
