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

//////////////////////////////////////////
/////Variable
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChessBoard")
	UStaticMeshComponent* MeshComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChessBoard")
	TSubclassOf<ABoardFloor> FloorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChessBoard")
	TSubclassOf<AChessPiece> PieceClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GamePlay")
	AChessPiece* SelectedPiece = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GamePlay")
	ABoardFloor* SelectedFloor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GamePlay")
	AChessPiece* TargetPiece = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GamePlay")
	ABoardFloor* TargetFloor = nullptr;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChessBoard")
	TArray<ABoardFloor*> BoardFloors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChessBoard")
	TArray<ABoardFloor*> MovableFloors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChessBoard")
	TArray<ABoardFloor*> AttackableFloors;
private:
	const int32 Chess_Num  = 8;
	bool bIsClickedOnce = false;
	bool bIsClickedTwice = false;

	UPROPERTY()
	class AChessPlayerController* Controller;

	TArray<TArray<int32>> KnightArray;
	TArray<TArray<int32>> KingArray;
//////////////////////////////////////////
/////FUNCTION
public:
	UFUNCTION()
	void ClickFloor();
	
	void MovePiece();

	UFUNCTION()
	void PieceEncounter(AChessPiece* Selected, AChessPiece* Target);

	void MoveEnd();
	
protected:
	UFUNCTION()
	ABoardFloor* SpawnFloor(int32 row, int32 col);

	UFUNCTION()
	void InitBoard();

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
private:
	
};