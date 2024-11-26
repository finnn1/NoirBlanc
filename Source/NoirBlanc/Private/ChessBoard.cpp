// Fill out your copyright notice in the Description page of Project Settings.
#include "ChessBoard.h"
#include "BoardFloor.h"
#include "ChessPiece.h"
#include "ChessPlayerController.h"

// Sets default values
AChessBoard::AChessBoard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
	
	BoardFloors.SetNum(Chess_Num * Chess_Num);

	KnightArray = KnightValue;
	
	KingArray = KingValue;
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
			TempFloor->SetRow(i);
			TempFloor->SetCol(j);
			BoardFloors[i*Chess_Num + j] = TempFloor;
		}
	}
	Controller = Cast<AChessPlayerController>(GetWorld()->GetFirstPlayerController());
	InitBoard();
}

// Called every frame
void AChessBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AChessBoard::ClickFloor()
{
	AActor* HitActor = Controller->TraceForActor();
	if(!bIsClickedOnce)
	{
		SelectedPiece = Cast<AChessPiece>(HitActor);
		if(SelectedPiece)
		{
			bIsClickedOnce = true;
			SelectedFloor = SelectedPiece->GetFloorBeneathPiece();
			SelectedFloor->ToggleGreen();
			ShowMovableFloors(SelectedFloor);
		}
	}
	else
	{
		//Add Function to Determine movable floors
		TargetFloor = Cast<ABoardFloor>(HitActor);
		TargetPiece = Cast<AChessPiece>(HitActor);

		//If Target is Piece
		if(TargetPiece)
		{
			bIsClickedTwice = true;
			TargetFloor = TargetPiece->GetFloorBeneathPiece();
			MovePiece();
		}
		//If Target is Floor
		else if(TargetFloor)
		{
			bIsClickedTwice = true;
			MovePiece();
		}
	}
}

void AChessBoard::MovePiece()
{
	//if piece is present at target floor
	if(TargetPiece)
	{
		PieceEncounter(SelectedPiece, TargetPiece);
	}
	//if target floor is empty
	else
	{
		SelectedFloor->SetPieceOnFloor(nullptr);
		SelectedPiece->SetFloorBeneathPiece(TargetFloor);
		TargetFloor->SetPieceOnFloor(SelectedPiece);
		SelectedPiece->SetActorLocation(TargetFloor->GetActorLocation() + FVector(0.f, 0.f, 190.f));
		MoveEnd();
	}
}

void AChessBoard::PieceEncounter(AChessPiece* Selected, AChessPiece* Target)
{
	//Event when two pieces meet
	if(Selected)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,  FString::Printf(TEXT("Pawn Game Start!")));
	}
	Selected->IncreaseEncounterCount();
	Target->IncreaseEncounterCount();
	MoveEnd();
}

void AChessBoard::MoveEnd()
{
	SelectedFloor->ToggleGreen();
	for(auto Floor : MovableFloors)
	{
		if(Floor->GetPieceOnFloor() == nullptr || Floor->GetPieceOnFloor() == SelectedPiece)
		{
			Floor->ToggleBlue();
		}
	}
	for(auto Floor : AttackableFloors)
	{
		Floor->ToggleRed();
	}
	MovableFloors.Empty();
	AttackableFloors.Empty();
	SelectedPiece->IncreaseMoveCount();
	SelectedFloor = nullptr;
	TargetFloor = nullptr;
	SelectedPiece = nullptr;
	TargetPiece = nullptr;
	bIsClickedOnce = false;
	bIsClickedTwice = false;
}

ABoardFloor* AChessBoard::SpawnFloor(int32 row, int32 col)
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
	//Pawn
	for(int i = 8; i < 8 + Chess_Num; i++)
	{
		InitPiece(i, EPieceType::Pawn, EPieceColor::Black);
	}

	//White Side
	for(int i =48; i < 48 + Chess_Num; i++)
	{
		InitPiece(i, EPieceType::Pawn, EPieceColor::White);
	}
	InitPiece(56, EPieceType::Rook, EPieceColor::White);
	InitPiece(57, EPieceType::Knight, EPieceColor::White);
	InitPiece(58, EPieceType::Bishop, EPieceColor::White);
	InitPiece(59, EPieceType::Queen, EPieceColor::White);
	InitPiece(60, EPieceType::King, EPieceColor::White);
	InitPiece(61, EPieceType::Bishop, EPieceColor::White);
	InitPiece(62, EPieceType::Knight, EPieceColor::White);
	InitPiece(63, EPieceType::Rook, EPieceColor::White);
}

void AChessBoard::InitPiece(int32 num, EPieceType type, EPieceColor color)
{
	FVector SpawnLocation = BoardFloors[num]->GetActorLocation() + FVector(0.f, 0.f, 190.f);
	FRotator SpawnRotation;
	if(color == EPieceColor::Black)
	{
		SpawnRotation = FRotator(0.0f, 0.f, 0.0f);
	}
	else
	{
		SpawnRotation = FRotator(0.0f, -180.f, 0.0f);
	}
	if(type == EPieceType::Bishop)
	{
		SpawnRotation += FRotator(0.f, 90.f, 0.0f);
	}
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	AChessPiece* SpawnedPiece = GetWorld()->SpawnActor<AChessPiece>(PieceClass, SpawnLocation, SpawnRotation, SpawnParams);
	SpawnedPiece->SetPieceColor(color);
	SpawnedPiece->SetPieceType(type);
	SpawnedPiece->SetFloorBeneathPiece(BoardFloors[num]);
	if (USceneComponent* RootComp = SpawnedPiece->GetRootComponent())
	{
		RootComp->SetWorldScale3D(FVector(100.f, 100.f, 100.f));
	}
	BoardFloors[num]->SetPieceOnFloor(SpawnedPiece);
}

void AChessBoard::ShowMovableFloors(ABoardFloor* Point)
{
	AChessPiece* PointPiece = Point->GetPieceOnFloor();
	int32 Row = Point->GetRow();
	int32 Col = Point->GetCol();
	int32 MoveCount = PointPiece->GetMoveCount();
	EPieceColor Color = PointPiece->GetPieceColor();
	EPieceType Type = PointPiece->GetPieceType();

	switch(Type)
	{
		case EPieceType::Pawn :
			ShowPawnFloors(Color, Row, Col, MoveCount);
			break;
		case EPieceType::Rook :
			ShowRookFloors(Color, Row, Col);
			break;
		case EPieceType::Knight :
			ShowKnightFloors(Color, Row, Col);
			break;
		case EPieceType::Bishop :
			ShowBishopFloors(Color, Row, Col);
			break;
		case EPieceType::Queen :
			ShowQueenFloors(Color, Row, Col);
			break;
		case EPieceType::King :
			ShowKingFloors(Color, Row, Col);
			break;
	}
	for(auto Floor : MovableFloors)
	{
		if(Floor->GetPieceOnFloor() == nullptr)
		{
			Floor->ToggleBlue();
		}
	}
	for(auto Floor : AttackableFloors)
	{
		Floor->ToggleRed();
	}
}

void AChessBoard::ShowPawnFloors(EPieceColor Color, int32 Row, int32 Col, int32 MoveCount)
{
	if(Color == EPieceColor::Black)
	{
		MovableFloors.Add(BoardFloors[(Row+1)*Chess_Num + Col]);
		if(MoveCount == 0)
		{
			MovableFloors.Add(BoardFloors[(Row+2)*Chess_Num + Col]);
		}
		if(Col-1 >= 0)
		{
			ABoardFloor* TempFloor = BoardFloors[(Row+1)*Chess_Num + Col-1];
			if(TempFloor->GetPieceOnFloor() != nullptr)
			{
				if(TempFloor->GetPieceOnFloor()->GetPieceColor() != EPieceColor::Black)
				{
					AttackableFloors.Add(TempFloor);
				}
			}
		}
		if(Col+1 <= Chess_Num -1 )
		{
			ABoardFloor* TempFloor = BoardFloors[(Row+1)*Chess_Num + Col+1];
			if(TempFloor->GetPieceOnFloor() != nullptr)
			{
				if(TempFloor->GetPieceOnFloor()->GetPieceColor() != EPieceColor::Black)
				{
					AttackableFloors.Add(TempFloor);
				}
			}
		}
	}
	else
	{
		MovableFloors.Add(BoardFloors[(Row-1)*Chess_Num + Col]);
		if(MoveCount == 0)
		{
			MovableFloors.Add(BoardFloors[(Row-2)*Chess_Num + Col]);
		}
		if(Col-1 >= 0)
		{
			ABoardFloor* TempFloor = BoardFloors[(Row-1)*Chess_Num + Col-1];
			if(TempFloor->GetPieceOnFloor() != nullptr)
			{
				if(TempFloor->GetPieceOnFloor()->GetPieceColor() != EPieceColor::White)
				{
					AttackableFloors.Add(TempFloor);
				}
			}
		}
		if(Col+1 <= Chess_Num -1 )
		{
			ABoardFloor* TempFloor = BoardFloors[(Row-1)*Chess_Num + Col+1];
			if(TempFloor->GetPieceOnFloor() != nullptr)
			{
				if(TempFloor->GetPieceOnFloor()->GetPieceColor() != EPieceColor::White)
				{
					AttackableFloors.Add(TempFloor);
				}
			}
		}
	}
}

void AChessBoard::ShowRookFloors(EPieceColor Color, int32 Row, int32 Col)
{
	int32 index;
	int32 counter = 0;
	//vertical
	while(counter < Chess_Num)
	{
		counter++;
		index = (Row-counter)*Chess_Num + Col;
		if(index >= 0 && index < Chess_Num * Chess_Num)
		{
			ABoardFloor* Temp = BoardFloors[index];
			if(Temp->GetPieceOnFloor() == nullptr)
			{
				MovableFloors.Add(Temp);
			}
			else if(Temp->GetPieceOnFloor()->GetPieceColor() == Color)
			{
				break;
			}
			else if(Temp->GetPieceOnFloor()->GetPieceColor() != Color)
			{
				AttackableFloors.Add(Temp);
				break;
			}
		}
	}
	counter = 0;
	while(counter < Chess_Num)
	{
		counter++;
		index = (Row+counter)*Chess_Num + Col;
		if(index >= 0 && index < Chess_Num * Chess_Num)
		{
			ABoardFloor* Temp = BoardFloors[index];
			if(Temp->GetPieceOnFloor() == nullptr)
			{
				MovableFloors.Add(Temp);
			}
			else if(Temp->GetPieceOnFloor()->GetPieceColor() == Color)
			{
				break;
			}
			else if(Temp->GetPieceOnFloor()->GetPieceColor() != Color)
			{
				AttackableFloors.Add(Temp);
				break;
			}
		}
	}
	//horizontal
	counter = 0;
	while(counter < Chess_Num)
	{
		counter++;
		index = Row*Chess_Num + (Col - counter);
		if(Col-counter < 0) break;
		if(index >= 0 && index < Chess_Num * Chess_Num)
		{
			ABoardFloor* Temp = BoardFloors[index];
			if(Temp->GetPieceOnFloor() == nullptr)
			{
				MovableFloors.Add(Temp);
			}
			else if(Temp->GetPieceOnFloor()->GetPieceColor() == Color)
			{
				break;
			}
			else if(Temp->GetPieceOnFloor()->GetPieceColor() != Color)
			{
				AttackableFloors.Add(Temp);
				break;
			}
		}
	}
	counter = 0;
	while(counter < Chess_Num)
	{
		counter++;
		index = Row*Chess_Num + (Col + counter);
		if(Col + counter >= Chess_Num) break;
		if(index >= 0 && index < Chess_Num * Chess_Num)
		{
			ABoardFloor* Temp = BoardFloors[index];
			if(Temp->GetPieceOnFloor() == nullptr)
			{
				MovableFloors.Add(Temp);
			}
			else if(Temp->GetPieceOnFloor()->GetPieceColor() == Color)
			{
				break;
			}
			else if(Temp->GetPieceOnFloor()->GetPieceColor() != Color)
			{
				AttackableFloors.Add(Temp);
				break;
			}
		}
	}
}

void AChessBoard::ShowKnightFloors(EPieceColor Color, int32 Row, int32 Col)
{
	int32 index;
	
	index = (Row + 2) * Chess_Num + Col;
	if(index >= 0 && index < Chess_Num * Chess_Num)
	{
		ABoardFloor* Temp = BoardFloors[index];
	}
	for(int i = 0 ; i< Chess_Num; i++)
	{
		int32 first = KnightArray[i][0];
		int32 second = KnightArray[i][1];

		index = (Row + first) * Chess_Num + Col + second;

		if(index >= 0 && index < Chess_Num * Chess_Num)
		{
			ABoardFloor* Temp = BoardFloors[index];
		
			if(Col + second >=0 && Col + second < Chess_Num)
			{
				if(Temp->GetPieceOnFloor() == nullptr)
				{
					MovableFloors.Add(Temp);
				}
				else if(Temp->GetPieceOnFloor()->GetPieceColor() == Color)
				{
					continue;
				}
				else if(Temp->GetPieceOnFloor()->GetPieceColor() != Color)
				{
					AttackableFloors.Add(Temp);
					continue;
				}
			}
		}
	}
}

void AChessBoard::ShowBishopFloors(EPieceColor Color, int32 Row, int32 Col)
{
	int32 index;
	int32 counter = 0;
	//left up
	while(counter < Chess_Num)
	{
		counter++;
		index = (Row+counter)*Chess_Num + (Col -counter);
		if(index >= 0 && index < Chess_Num * Chess_Num)
		{
			ABoardFloor* Temp = BoardFloors[index];
			if(Col -counter >= 0 && Col -counter < Chess_Num)
			{
				if(Temp->GetPieceOnFloor() == nullptr)
				{
					MovableFloors.Add(Temp);
				}
				else if(Temp->GetPieceOnFloor()->GetPieceColor() == Color)
				{
					break;
				}
				else if(Temp->GetPieceOnFloor()->GetPieceColor() != Color)
				{
					AttackableFloors.Add(Temp);
					break;
				}
			}
		}
	}
	//left down
	counter = 0;
	while(counter < Chess_Num)
	{
		counter++;
		index = (Row-counter)*Chess_Num + (Col -counter);
		if(index >= 0 && index < Chess_Num * Chess_Num)
		{
			ABoardFloor* Temp = BoardFloors[index];
			if(Col -counter >= 0 && Col -counter < Chess_Num)
			{
				if(Temp->GetPieceOnFloor() == nullptr)
				{
					MovableFloors.Add(Temp);
				}
				else if(Temp->GetPieceOnFloor()->GetPieceColor() == Color)
				{
					break;
				}
				else if(Temp->GetPieceOnFloor()->GetPieceColor() != Color)
				{
					AttackableFloors.Add(Temp);
					break;
				}
			}
		}
	}
	//right up
	counter = 0;
	while(counter < Chess_Num)
	{
		counter++;
		index = (Row+counter)*Chess_Num + (Col +counter);
		if(index >= 0 && index < Chess_Num * Chess_Num)
		{
			ABoardFloor* Temp = BoardFloors[index];
			if(Col +counter >= 0 && Col +counter < Chess_Num)
			{
				if(Temp->GetPieceOnFloor() == nullptr)
				{
					MovableFloors.Add(Temp);
				}
				else if(Temp->GetPieceOnFloor()->GetPieceColor() == Color)
				{
					break;
				}
				else if(Temp->GetPieceOnFloor()->GetPieceColor() != Color)
				{
					AttackableFloors.Add(Temp);
					break;
				}
			}
		}
	}
	//right down
	counter = 0;
	while(counter < Chess_Num)
	{
		counter++;
		index = (Row-counter)*Chess_Num + (Col+counter);
		if(index >= 0 && index < Chess_Num * Chess_Num)
		{
			ABoardFloor* Temp = BoardFloors[index];
			if(Col +counter >= 0 && Col +counter < Chess_Num)
			{
				if(Temp->GetPieceOnFloor() == nullptr)
				{
					MovableFloors.Add(Temp);
				}
				else if(Temp->GetPieceOnFloor()->GetPieceColor() == Color)
				{
					break;
				}
				else if(Temp->GetPieceOnFloor()->GetPieceColor() != Color)
				{
					AttackableFloors.Add(Temp);
					break;
				}
			}
		}
	}
}

void AChessBoard::ShowQueenFloors(EPieceColor Color, int32 Row, int32 Col)
{
	ShowBishopFloors(Color, Row, Col);
	ShowRookFloors(Color, Row, Col);
}

void AChessBoard::ShowKingFloors(EPieceColor Color, int32 Row, int32 Col)
{
	int32 index;
	
	index = (Row + 2) * Chess_Num + Col;
	if(index >= 0 && index < Chess_Num * Chess_Num)
	{
		ABoardFloor* Temp = BoardFloors[index];
	}
	for(int i = 0 ; i< Chess_Num; i++)
	{
		int32 first = KingArray[i][0];
		int32 second = KingArray[i][1];

		index = (Row + first) * Chess_Num + Col + second;

		if(index >= 0 && index < Chess_Num * Chess_Num)
		{
			ABoardFloor* Temp = BoardFloors[index];
		
			if(Col + second >=0 && Col + second < Chess_Num)
			{
				if(Temp->GetPieceOnFloor() == nullptr)
				{
					MovableFloors.Add(Temp);
				}
				else if(Temp->GetPieceOnFloor()->GetPieceColor() == Color)
				{
					continue;
				}
				else if(Temp->GetPieceOnFloor()->GetPieceColor() != Color)
				{
					AttackableFloors.Add(Temp);
					continue;
				}
			}
		}
	}
}