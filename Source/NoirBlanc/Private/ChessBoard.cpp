// Fill out your copyright notice in the Description page of Project Settings.
#include "ChessBoard.h"
#include "BoardFloor.h"
#include "ChessPiece.h"
#include "ChessPlayerController.h"
#include "NoirBlancGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"
#include "Net/UnrealNetwork.h"
#include "NoirBlanc/Knight/TurnUI.h"
#include "QueenSelectWidget.h"
#include "NoirBlanc/Knight/BattleUI.h"
#include "NoirBlanc/Knight/ResultUI.h"

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

void AChessBoard::BeginPlay()
{
	Super::BeginPlay();
	
	GameInstance = Cast<UNoirBlancGameInstance>(GetWorld()->GetGameInstance());
	Controller = Cast<AChessPlayerController>(GetWorld()->GetFirstPlayerController());;
	
	//need to add Condition to Start Game When Possesed 
	StartGame();
}

void AChessBoard::StartGame()
{
	TurnUI = CreateWidget<UTurnUI>(GetWorld(), TurnUIClass);
	FTimerHandle UITimerHandle;
	GetWorld()->GetTimerManager().SetTimer(UITimerHandle, [this](){TurnUI->AddToViewport();}, 1.f, false);
	FTimerHandle TurnTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TurnTimerHandle, this, &AChessBoard::ServerRPC_TurnUIChange, 3.f, false);
	FTimerHandle MiniGameTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(MiniGameTimerHandle, this, &AChessBoard::ServerRPC_MiniGameEnd, 4.f, false);
	PlaySound(BackgroundMusic);

	ResultUI = Cast<UResultUI>(CreateWidget(GetWorld(), ResultUIClass));
	ResultUI->AddToViewport();
	ResultUI->ShowResult(EPieceType::King, EPieceColor::Black,
						EPieceType::King, EPieceColor::White,
						EPieceColor::Black);
}

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
	DOREPLIFETIME(AChessBoard, Turn);
}

void AChessBoard::ClickFloor()
{
	AActor* HitActor = Controller->TraceForActor();
	if(!bIsClickedOnce)
	{
		SelectedPiece = Cast<AChessPiece>(HitActor);
		if(SelectedPiece)
		{
			if(SelectedPiece->GetPieceColor() == Turn)
			{
				if(HasAuthority() && SelectedPiece->GetPieceColor() == EPieceColor::White)
				{
					PlaySound(PiecePickSound);
					bIsClickedOnce = true;
					SelectedFloor = SelectedPiece->GetFloorBeneathPiece();
					ServerRPC_SetPiece(SelectedFloor, TargetFloor, SelectedPiece, TargetPiece, MovableFloors, AttackableFloors);
					SelectedFloor->ToggleGreen();
					ShowMovableFloors(SelectedFloor);
				}
				else if(!HasAuthority() && SelectedPiece->GetPieceColor() == EPieceColor::Black)
				{
					PlaySound(PiecePickSound);
					bIsClickedOnce = true;
					SelectedFloor = SelectedPiece->GetFloorBeneathPiece();
					ServerRPC_SetPiece(SelectedFloor, TargetFloor, SelectedPiece, TargetPiece, MovableFloors, AttackableFloors);
					SelectedFloor->ToggleGreen();
					ShowMovableFloors(SelectedFloor);
				}
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
			//Event when Selected and Target are Equal
			if(SelectedPiece == TargetPiece)
			{
				SamePieceClicked();
				return;
			}
			ServerRPC_SetPiece(SelectedFloor, TargetFloor, SelectedPiece, TargetPiece, MovableFloors, AttackableFloors);
			ServerRPC_MovePiece();
		}
		//If Target is Floor
		else if(TargetFloor)
		{
			bIsClickedTwice = true;
			ServerRPC_SetPiece(SelectedFloor, TargetFloor, SelectedPiece, TargetPiece, MovableFloors, AttackableFloors);
			ServerRPC_MovePiece();
		}
	}
}

void AChessBoard::ServerRPC_SetPiece_Implementation(ABoardFloor* _SelectedFloor, ABoardFloor* _TargetFloor, AChessPiece* _SelectedPiece, AChessPiece* _TargetPiece,  const TArray<ABoardFloor*>& _MovableFloors,  const TArray<ABoardFloor*>& _AttackableFloors)
{
	SelectedFloor = _SelectedFloor;
	TargetFloor = _TargetFloor;
	SelectedPiece = _SelectedPiece;
	TargetPiece = _TargetPiece;
	MovableFloors = _MovableFloors;
	AttackableFloors = _AttackableFloors;
}

void AChessBoard::ServerRPC_MovePiece_Implementation()
{
	MulticastRPC_MovePiece();
}

void AChessBoard::MulticastRPC_MovePiece_Implementation()
{
	MovePiece();
}

void AChessBoard::MovePiece()
{
	//if piece is present at target floor
	if(TargetPiece)
	{
		ServerRPC_PieceEncounter();
	}
	//if target floor is empty
	else
	{
		for(int32 i = 0 ; i < MovableFloors.Num(); i++)
		{
			//if target floor is movable
			if(MovableFloors[i] == TargetFloor)
			{
				int32 index = FMath::RandRange(0, PiecePutSounds.Num()-1);
				PlaySound(PiecePutSounds[index]);
				SelectedFloor->SetPieceOnFloor(nullptr);
				SelectedPiece->SetFloorBeneathPiece(TargetFloor);
				TargetFloor->SetPieceOnFloor(SelectedPiece);
				SelectedPiece->SetActorLocation(TargetFloor->GetActorLocation() + FVector(0.f, 0.f, SpawnHeight));
				SelectedPiece->IncreaseMoveCount();
				MoveEnd();
			}
		}
	}
}

void AChessBoard::ServerRPC_PieceEncounter_Implementation()
{
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle,[this](){MulticastRPC_PieceEncounter();}, 0.3f ,false);
	
}

void AChessBoard::MulticastRPC_PieceEncounter_Implementation()
{
	PieceEncounter(SelectedPiece, TargetPiece);
}

void AChessBoard::PieceEncounter(AChessPiece* Selected, AChessPiece* Target)
{
	//Event when two pieces meet
	for(auto Floor : AttackableFloors)
	{
		if(Floor->GetPieceOnFloor() == Target)
		{
			Selected->IncreaseEncounterCount();
			Target->IncreaseEncounterCount();
			if(Selected->GetPieceType() == EPieceType::Queen)
			{
				QueenEncounter();
			}
			else
			{	
				AfterQueen(Selected, Target);
			}
		}
	}
}


void AChessBoard::QueenEncounter()
{
	if(Turn == EPieceColor::White)
	{
		if(HasAuthority())
		{
			ShowQueenWidget();
		}	
	}
	else if(Turn == EPieceColor::Black)
	{
		if(!HasAuthority())
		{
			ShowQueenWidget();
		}
	}
}

void AChessBoard::ServerRPC_AfterQueen_Implementation(const FString& Level)
{
	QueenLevel = Level;
	MulticastRPC_AfterQueen();
}

void AChessBoard::MulticastRPC_AfterQueen_Implementation()
{
	AfterQueen(SelectedPiece, TargetPiece);
}

void AChessBoard::AfterQueen(AChessPiece* Selected, AChessPiece* Target)
{
	DestroyQueenWidget();
	FName LevelName;
	EPieceType KingGame;
	EPieceType Game = Selected->GetPieceType();
	if(Target->GetPieceType() == EPieceType::King)
	{
		 KingGame = Target->GetPieceType();
	}
	//Save Data to GameInstance
	GameInstance->DeffenderColor = Target->GetPieceColor();
	GameInstance->DeffenderType = Target->GetPieceType();
	GameInstance->AttackerColor = Selected->GetPieceColor();
	GameInstance->AttackerType = Target->GetPieceType();
	GameInstance->AttackerRow = SelectedFloor->GetRow();
	GameInstance->AttackerCol = SelectedFloor->GetCol();
	GameInstance->DeffenderRow = TargetFloor->GetRow();
	GameInstance->DeffenderCol = TargetFloor->GetCol();
	bIsGoingToAnotherLevel = true;
	MoveEnd();
	GameInstance->Saved_Turn = Turn;
	//Contact Begin
	PlaySound(BattleSound);
	BattleUI = CreateWidget<UBattleUI>(GetWorld(), BattleUIClass);
	BattleUI->AddToViewport();

	if(HasAuthority())
	{
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
		if(KingGame == EPieceType::King)
		{
			LevelName = "King";
		}
		else
		{
			switch(Game)
			{
			case EPieceType::Pawn:
				LevelName = "Pawn";
				break;
			case EPieceType::Knight:
				LevelName = "Knight";
				break;
			case EPieceType::Bishop:
				LevelName = "Bishop";
				break;
			case EPieceType::Rook:
				LevelName = "Rook";
				break;
			case EPieceType::Queen:
				LevelName = FName(*QueenLevel);
				break;
			}
		}
		LevelToOpen = LevelName;
		OpenLevel();
	}
}

void AChessBoard::ServerRPC_MiniGameEnd_Implementation()
{
	MulticastRPC_MiniGameEnd();
}

void AChessBoard::MulticastRPC_MiniGameEnd_Implementation()
{
	MiniGameEnd();
}

void AChessBoard::MiniGameEnd()
{
	EPieceColor Winner = GameInstance->WinnerColor;
	int32 Delete_Row;
	int32 Delete_Col;)
	
	if(GameInstance->DeffenderColor == Winner)
	{
		Delete_Row = GameInstance->AttackerRow;
		Delete_Col = GameInstance->AttackerCol;
		DeletePiece(BoardFloors[Delete_Row*Chess_Num + Delete_Col]->GetPieceOnFloor());
	}
	else if(GameInstance->AttackerColor == Winner)
	{
		Delete_Col = GameInstance->DeffenderCol;
		Delete_Row = GameInstance->DeffenderRow;
		DeletePiece(BoardFloors[Delete_Row*Chess_Num + Delete_Col]->GetPieceOnFloor());

		AChessPiece* Attacker = BoardFloors[GameInstance->AttackerRow*Chess_Num + GameInstance->AttackerCol]->GetPieceOnFloor();
		ABoardFloor* Destination = BoardFloors[Delete_Row*Chess_Num + Delete_Col];
		Attacker->SetFloorBeneathPiece(Destination);
		Destination->SetPieceOnFloor(Attacker);
		FTimerHandle Timer;
		GetWorld()->GetTimerManager().SetTimer(Timer, [Attacker, Destination, this](){
			Attacker->SetActorLocation(Destination->GetActorLocation() + FVector(0.f, 0.f, SpawnHeight);
		int32 index = FMath::RandRange(0, PiecePutSounds.Num()-1);
		PlaySound(PiecePutSounds[index]);}, 1.5f, false);
	}
}

void AChessBoard::MoveEnd()
{
	ChangeTurn();
	SelectedFloor->DeactivateGreen();
	for(auto Floor : MovableFloors)
	{
		if(Floor->GetPieceOnFloor() == nullptr || Floor->GetPieceOnFloor() == SelectedPiece)
		{
			Floor->DeactivateBlue();
		}
	}
	for(auto Floor : AttackableFloors)
	{
		Floor->DeactivateRed();
	}
	MovableFloors.Empty();
	AttackableFloors.Empty();
	SelectedFloor = nullptr;
	TargetFloor = nullptr;
	SelectedPiece = nullptr;
	TargetPiece = nullptr;
	bIsClickedOnce = false;
	bIsClickedTwice = false;
	if(!bIsSamePieceClicked && !bIsGoingToAnotherLevel)
	{
		FTimerHandle TurnTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TurnTimerHandle, this, &AChessBoard::ServerRPC_TurnUIChange, 0.5f, false);
	}
	bIsSamePieceClicked = false;
	bIsGoingToAnotherLevel = false;
}

void AChessBoard::SamePieceClicked()
{
	bIsClickedOnce = false;
	bIsSamePieceClicked = true;
	ChangeTurn();
	MoveEnd();
}

void AChessBoard::ChangeTurn()
{
	if(Turn == EPieceColor::Black)
	{
		Turn = EPieceColor::White;
	}
	else
	{
		Turn = EPieceColor::Black;
	}
}

ABoardFloor* AChessBoard::SpawnFloor(int32 row, int32 col)
{
	if (UWorld* World = GetWorld()) 
	{
		FVector SpawnLocation = GetActorLocation() + FVector(col * 475.0f, row * 475.0f,  0.f) - FVector(1675.f, 1370.f ,0.f);
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

void AChessBoard::SetPieceData(int32 num, EPieceType type, EPieceColor color)
{
	AChessPiece* Piece = BoardFloors[num]->GetPieceOnFloor();
	Piece->SetPieceColor(color);
	Piece->SetPieceType(type);
	Piece->SetFloorBeneathPiece(BoardFloors[num]);
	Piece->SetMoveCount(GameInstance->MoveCountData[num]);
	Piece->SetEncounterCount(GameInstance->EncounterCountData[num]);
	
	if (USceneComponent* RootComp = Piece->GetRootComponent())
	{
		RootComp->SetWorldScale3D(FVector(100.f, 100.f, 100.f));
	}
}

void AChessBoard::InitFloor()
{
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
}

void AChessBoard::InitBoard()
{
	EPieceType Type;
	EPieceColor Color;
	Turn = GameInstance->Saved_Turn;
	
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
	FVector SpawnLocation = BoardFloors[num]->GetActorLocation() + FVector(0.f, 0.f, SpawnHeight);
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
	
	BoardFloors[num]->SetPieceOnFloor(SpawnedPiece);
	SetPieceData(num, type, color);
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

void AChessBoard::PlaySound(USoundBase* Sound)
{
	if (Sound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), Sound);
	}
}

void AChessBoard::TurnUIChange()
{
	TurnUI->ShowTurn(Turn);
}

void AChessBoard::DeletePiece(AChessPiece* DeletePiece)
{
	//Material Change
	if(DeletePiece)
	{
		if(DeletePiece->GetPieceType() == EPieceType::King)
		{
			ServerRPC_EndGame(DeletePiece->GetPieceColor());

		}

		DeletePiece->DissolveMaterial();

		FTimerHandle DeleteTimer;
		GetWorld()->GetTimerManager().SetTimer(DeleteTimer, [DeletePiece](){DeletePiece->Destroy();}, 2.15f, false);
	}
}

void AChessBoard::EndGame(EPieceColor Loser)
{
	
}

void AChessBoard::MulticastRPC_EndGame_Implementation(EPieceColor Loser)
{
	EndGame(Loser);
}

void AChessBoard::ServerRPC_EndGame_Implementation(EPieceColor Loser)
{
	MulticastRPC_EndGame(Loser);
}

void AChessBoard::ShowQueenWidget()
{
	FString Selected;
	if (Controller)
	{
		QueenSelectUI = CreateWidget<UQueenSelectWidget>(GetWorld(), QueenSelectUIClass);
		if (QueenSelectUI) 
		{
			QueenSelectUI->AddToViewport();
	        FInputModeUIOnly InputMode;
	        Controller->SetInputMode(InputMode);
			UGameplayStatics::SetGamePaused(GetWorld(), true);
		}
	}
}

void AChessBoard::DestroyQueenWidget()
{
	if(QueenSelectUI)
	{
		QueenSelectUI->RemoveFromParent();
		QueenSelectUI = nullptr;
		if (Controller)
		{
			FInputModeGameOnly InputMode;
			Controller->SetInputMode(InputMode);
		}
	}
}

void AChessBoard::QueenWidgetClicked(const FString& Level)
{
	ServerRPC_AfterQueen(Level);
}

void AChessBoard::OpenLevel()
{
	FString ServerURL = *GameLevelMap.Find(LevelToOpen);
	ServerURL += TEXT("?listen");
				
	FTimerHandle LevelTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(LevelTimerHandle, [this, ServerURL](){
		if (ServerURL.IsEmpty() == false)
		{
			Controller->ServerRPC_LevelTravel(ServerURL);
		}}, 2.f, false);
}

void AChessBoard::ServerRPC_TurnUIChange_Implementation()
{
	MulticastRPC_TurnUIChange();
}

void AChessBoard::MulticastRPC_TurnUIChange_Implementation()
{
	TurnUIChange();
}