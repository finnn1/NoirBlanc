// Fill out your copyright notice in the Description page of Project Settings.
#include "ChessBoard.h"
#include "BoardFloor.h"
#include "ChessPiece.h"
#include "ChessPlayerController.h"
#include "NoirBlancGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

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
	GameInstance = Cast<UNoirBlancGameInstance>(GetWorld()->GetGameInstance());
	Controller = Cast<AChessPlayerController>(GetWorld()->GetFirstPlayerController());
	InitBoard();
}

// Called every frame
void AChessBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AChessBoard::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AChessBoard, BoardFloors);
	DOREPLIFETIME(AChessBoard, MovableFloors);
	DOREPLIFETIME(AChessBoard, AttackableFloors);
	DOREPLIFETIME(AChessBoard, SelectedPiece);
	DOREPLIFETIME(AChessBoard, SelectedFloor);
	DOREPLIFETIME(AChessBoard, TargetPiece);
	DOREPLIFETIME(AChessBoard, TargetFloor);
}

void AChessBoard::ClickFloor()
{
	AActor* HitActor = Controller->TraceForActor();
	if(!bIsClickedOnce)
	{
		SelectedPiece = Cast<AChessPiece>(HitActor);
		if(SelectedPiece)
		{
			if(SelectedPiece->GetPieceColor() == GameInstance->Turn)
			{
				bIsClickedOnce = true;
				SelectedFloor = SelectedPiece->GetFloorBeneathPiece();
				SelectedFloor->ToggleGreen();
				ShowMovableFloors(SelectedFloor);
			}
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
			bIsTargetPointEmpty = false;
			TargetFloor = TargetPiece->GetFloorBeneathPiece();
			ServerRPC_MovePiece();
		}
		//If Target is Floor
		else if(TargetFloor)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Check1"));
			bIsClickedTwice = true;
			SetOwner(GetWorld()->GetFirstPlayerController()->GetPawn());
			UE_LOG(LogTemp, Warning, TEXT("%s"), *this->GetOwner()->GetName());
			ServerRPC_MovePiece();
		}
	}
}

void AChessBoard::ServerRPC_MovePiece_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Check1"));
	MulticastRPC_MovePiece();
}

void AChessBoard::MulticastRPC_MovePiece_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Check2"));
	MovePiece();
}

void AChessBoard::MovePiece()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Check"));
	//if piece is present at target floor
	if(TargetPiece)
	{
		PieceEncounter(SelectedPiece, TargetPiece);
	}
	//if target floor is empty
	else
	{
		for(int32 i = 0 ; i < MovableFloors.Num(); i++)
		{
			//if target floor is movable
			if(MovableFloors[i] == TargetFloor)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Check"));
				SelectedFloor->SetPieceOnFloor(nullptr);
				SelectedPiece->SetFloorBeneathPiece(TargetFloor);
				TargetFloor->SetPieceOnFloor(SelectedPiece);
				SelectedPiece->SetActorLocation(TargetFloor->GetActorLocation() + FVector(0.f, 0.f, 200.f));
				SelectedPiece->IncreaseMoveCount();
				MoveEnd();
			}
		}
	}
}

void AChessBoard::ServerRPC_PieceEncounter_Implementation()
{
}

void AChessBoard::MulticastRPC_PieceEncounter_Implementation()
{
}

void AChessBoard::PieceEncounter(AChessPiece* Selected, AChessPiece* Target)
{
	//Event when Selected and Target are Equal
	if(Selected == Target)
	{
		MoveEnd();
		ChangeTurn();
		return;
	}
	//Event when two pieces meet
	for(auto Floor : AttackableFloors)
	{
		if(Floor->GetPieceOnFloor() == Target)
		{
			FName LevelName;
			EPieceType Game = Selected->GetPieceType();
			GameInstance->DeffenderColor = Target->GetPieceColor();
			GameInstance->DeffenderType = Target->GetPieceType();
			GameInstance->AttackerColor = Selected->GetPieceColor();
			GameInstance->AttackerType = Target->GetPieceType();
			for(int i = 0 ; i < BoardFloors.Num(); i++)
			{
				//search through the board and save data
				if(BoardFloors[i]->GetPieceOnFloor())
				{
					GameInstance->BoardTypeData[i] = BoardFloors[i]->GetPieceOnFloor()->GetPieceType();
					GameInstance->BoardColorData[i] = BoardFloors[i]->GetPieceOnFloor()->GetPieceColor();
					GameInstance->EncounterCountData[i] = BoardFloors[i]->GetPieceOnFloor()->GetEncounterCount();
					GameInstance->MoveCountData[i] = BoardFloors[i]->GetPieceOnFloor()->GetMoveCount();
				}
				else
				{
					GameInstance->BoardTypeData[i] = EPieceType::Blank;
					GameInstance->BoardColorData[i] = EPieceColor::Blank;
				}
			}
			switch(Game)
			{
			case EPieceType::Pawn:
				LevelName = "Lv_PawnCard";
				break;
			case EPieceType::Knight:
				LevelName = "Lv_Knight";
				break;
			case EPieceType::Bishop:
				LevelName = "Lv_Bishop";
				break;
			case EPieceType::Rook:
				LevelName = "Lv_Fortress";
				break;
			case EPieceType::Queen:
				break;
			case EPieceType::King:
				break;
			}
			Selected->IncreaseEncounterCount();
			Target->IncreaseEncounterCount();
			MoveEnd();
			UGameplayStatics::OpenLevel(this, LevelName);
		}
	}
}

void AChessBoard::MoveEnd()
{
	ChangeTurn();
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
	SelectedFloor = nullptr;
	TargetFloor = nullptr;
	SelectedPiece = nullptr;
	TargetPiece = nullptr;
	bIsClickedOnce = false;
	bIsClickedTwice = false;
}

void AChessBoard::ChangeTurn()
{
	if(GameInstance->Turn == EPieceColor::Black)
	{
		GameInstance->Turn = EPieceColor::White;
	}
	else
	{
		GameInstance->Turn = EPieceColor::Black;
	}
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
	EPieceType Type;
	EPieceColor Color;
	for(int i = 0 ; i < Chess_Num; i++)
	{
		for(int j = 0 ; j < Chess_Num; j++)
		{
			Type = GameInstance->BoardTypeData[i*Chess_Num+j];
			Color = GameInstance->BoardColorData[i*Chess_Num+j];

			if(Type!= EPieceType::Blank)
			{
				InitPiece(i*Chess_Num + j, Type, Color);
			}
		}
	}
}

void AChessBoard::InitPiece(int32 num, EPieceType type, EPieceColor color)
{
	FVector SpawnLocation = BoardFloors[num]->GetActorLocation() + FVector(0.f, 0.f, 250.f);
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
	SpawnedPiece->SetMoveCount(GameInstance->MoveCountData[num]);
	SpawnedPiece->SetEncounterCount(GameInstance->EncounterCountData[num]);
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
		int32 index = (Row+1)*Chess_Num + Col;
		if(BoardFloors[index]->GetPieceOnFloor() == nullptr)
		{
			MovableFloors.Add(BoardFloors[index]);
			if(MoveCount == 0)
			{
				index = (Row+2)*Chess_Num + Col;
				if(BoardFloors[index]->GetPieceOnFloor() == nullptr)
				{
					MovableFloors.Add(BoardFloors[index]);
				}
			}
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
		int32 index = (Row-1)*Chess_Num + Col;
		if(BoardFloors[index]->GetPieceOnFloor() == nullptr)
		{
			MovableFloors.Add(BoardFloors[index]);
			if(MoveCount == 0)
			{
				index = (Row-2)*Chess_Num + Col;
				if(BoardFloors[index]->GetPieceOnFloor() == nullptr)
				{
					MovableFloors.Add(BoardFloors[index]);
				}
			}
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