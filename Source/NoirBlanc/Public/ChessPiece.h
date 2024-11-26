// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PieceTypes.h"

#include "ChessPiece.generated.h"

class ABoardFloor;
class UBoxComponent;
UCLASS()
class NOIRBLANC_API AChessPiece : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChessPiece();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	EPieceType GetPieceType();
	EPieceColor GetPieceColor();
	ABoardFloor* GetFloorBeneathPiece();
	void SetPieceType(EPieceType Type);
	void SetPieceColor(EPieceColor Color);
	void SetFloorBeneathPiece(ABoardFloor* Floor);
	void IncreaseMoveCount();
	void IncreaseEncounterCount();
	int32 GetMoveCount();
	int32 GetEncounterCount();
	
protected:
	void SetPieceMesh();
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CompMesh")
	class UStaticMeshComponent* CompMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CompCollision")
	UBoxComponent* CompBox;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PieceMesh")
	TArray<UStaticMesh*> Meshes;

private:
	UPROPERTY(VisibleAnywhere,Category = "PieceInfo")
	EPieceType PieceType;

	UPROPERTY(VisibleAnywhere,Category = "PieceInfo")
	EPieceColor PieceColor;

	UPROPERTY(VisibleAnywhere,Category = "PieceInfo")
	ABoardFloor* FloorBeneathPiece;

	UPROPERTY(VisibleAnywhere,Category = "PieceInfo")
	int32 MoveCount = 0;
	
	UPROPERTY(VisibleAnywhere,Category = "PieceInfo")
	int32 EncounterCount = 0;
};
