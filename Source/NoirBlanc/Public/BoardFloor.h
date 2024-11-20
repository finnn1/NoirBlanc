// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoardFloor.generated.h"

class AChessPiece;
UCLASS()
class NOIRBLANC_API ABoardFloor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoardFloor();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	AChessPiece* GetPieceOnFloor();
	void SetPieceOnFloor(AChessPiece* Piece);
	int32 GetRow();
	int32 GetCol();
	void SetRow(int32 Row);
	void SetCol(int32 Col);

private:
	UPROPERTY(VisibleAnywhere, Category = "FloorInfo")
	AChessPiece* PieceOnFloor = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "FloorInfo")
	int32 FloorRow;

	UPROPERTY(VisibleAnywhere, Category = "FloorInfo")
	int32 FloorCol;
};
