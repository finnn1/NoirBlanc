// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PieceTypes.h"

#include "ChessBoard.generated.h"

class ABoardFloor;
class AChessPiece;
UCLASS()
class NOIRBLANC_API AChessBoard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChessBoard();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

//////////////////////////////////////////
/////Variable
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChessBoard")
	UStaticMeshComponent* MeshComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChessBoard")
	TSubclassOf<ABoardFloor> FloorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChessBoard")
	TSubclassOf<AChessPiece> PieceClass;

	UPROPERTY(Replicated,EditAnywhere, BlueprintReadWrite, Category = "GamePlay")
	AChessPiece* SelectedPiece = nullptr;

	UPROPERTY(Replicated,EditAnywhere, BlueprintReadWrite, Category = "GamePlay")
	ABoardFloor* SelectedFloor = nullptr;

	UPROPERTY(Replicated,EditAnywhere, BlueprintReadWrite, Category = "GamePlay")
	AChessPiece* TargetPiece = nullptr;
	
	UPROPERTY(Replicated,EditAnywhere, BlueprintReadWrite, Category = "GamePlay")
	ABoardFloor* TargetFloor = nullptr;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ChessBoard")
	bool bIsSamePieceClicked = false;
	
protected:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "ChessBoard")
	TArray<ABoardFloor*> BoardFloors;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "ChessBoard")
	TArray<ABoardFloor*> MovableFloors;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "ChessBoard")
	TArray<ABoardFloor*> AttackableFloors;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "ChessBoard")
	EPieceColor Turn = EPieceColor::White;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* PiecePickSound;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TArray<USoundBase*> PiecePutSounds;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* BackgroundMusic;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* BattleSound;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* WinningSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UTurnUI> TurnUIClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UTurnUI* TurnUI;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UResultUI> ResultUIClass;
	UResultUI* ResultUI;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UQueenSelectWidget> QueenSelectUIClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UQueenSelectWidget* QueenSelectUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UBattleUI> BattleUIClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UBattleUI* BattleUI;
	
private:
	const int32 Chess_Num  = 8;
	bool bIsClickedOnce = false;
	bool bIsClickedTwice = false;
	
	TArray<TArray<int32>> KnightArray;
	TArray<TArray<int32>> KingArray;
	
	UPROPERTY()
	class AChessPlayerController* Controller;

	UPROPERTY()
	class UNoirBlancGameInstance* GameInstance;

	bool bIsTargetPointEmpty = false;

	float SpawnHeight = 50.f;

	//bool bIsSamePieceClicked = false;

	bool bIsGoingToAnotherLevel = false;

	FName LevelToOpen;

	FString QueenLevel;
//////////////////////////////////////////
/////FUNCTION
public:
	UFUNCTION()
	void ClickFloor();

	UFUNCTION(Server, Reliable)
	void ServerRPC_SetPiece(ABoardFloor* _SelectedFloor, ABoardFloor* _TargetFloor, AChessPiece* _SelectedPiece, AChessPiece* _TargetPiece,  const TArray<ABoardFloor*>& _MovableFloors,  const TArray<ABoardFloor*>& _AttackableFloors);
	UFUNCTION(Server, Reliable)
	void ServerRPC_MovePiece();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_MovePiece();
	void MovePiece();

	UFUNCTION(Server, Reliable)
	void ServerRPC_PieceEncounter();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_PieceEncounter();
	UFUNCTION()
	void PieceEncounter(AChessPiece* Selected, AChessPiece* Target);
	
	void MoveEnd();

	void SamePieceClicked();
	
	UFUNCTION()
	void InitBoard();
	
	UFUNCTION()
	void InitFloor();

	void SetPieceData(int32 num, EPieceType type, EPieceColor color);
	
	UFUNCTION()
	void OpenLevel();
protected:
	UFUNCTION()
	void ChangeTurn();
	
	UFUNCTION()
	ABoardFloor* SpawnFloor(int32 row, int32 col);
	
	UFUNCTION()
	void InitPiece(int32 num, EPieceType type, EPieceColor color);
	
	UFUNCTION()
	void ShowMovableFloors(ABoardFloor* Point);

	UFUNCTION()
	void ShowPawnFloors(EPieceColor Color, int32 Row, int32 Col, int32 MoveCount);

	UFUNCTION()
	void ShowRookFloors(EPieceColor Color, int32 Row, int32 Col);

	UFUNCTION()
	void ShowKnightFloors(EPieceColor Color, int32 Row, int32 Col);

	UFUNCTION()
	void ShowBishopFloors(EPieceColor Color, int32 Row, int32 Col);

	UFUNCTION()
	void ShowQueenFloors(EPieceColor Color, int32 Row, int32 Col);

	UFUNCTION()
	void ShowKingFloors(EPieceColor Color, int32 Row, int32 Col);

	UFUNCTION()
	void PlaySound(USoundBase* Sound);

	UFUNCTION()
	void TurnUIChange();
	UFUNCTION(Server, Reliable)
	void ServerRPC_TurnUIChange();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_TurnUIChange();

public:
	UFUNCTION()
	void ShowQueenWidget();
	UFUNCTION()
	void DestroyQueenWidget();
	UFUNCTION()
	void QueenWidgetClicked(const FString& Level);
	
	void QueenEncounter();
	void AfterQueen(AChessPiece* Selected, AChessPiece* Target);

	void StartGame();
private:
};