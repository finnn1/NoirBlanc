// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PieceTypes.h"

#include "ChessBoard.generated.h"

class ABoardFloor;
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

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChessBoard")
	TArray<EPieceType> PiecesOnBoard;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChessBoard")
	TArray<ABoardFloor*> BoardFloors;

private:
	const int32 Chess_Num  = 8;
};